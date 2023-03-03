#ifndef INCLUDE_SORT_HELPERS_H
#define INCLUDE_SORT_HELPERS_H

#include <compare>
#include <functional>

#include "generic/named_type.h"
#include "table_helpers.h"

namespace table
{
    using colIndex_t = NamedType<int, struct columnIndex>;
    using policyFunc_f = NamedType<std::function<std::weak_ordering(const row_t &, const row_t &)>, struct PolicyFunction>;
    using compareFunc_f = NamedType<std::function<std::weak_ordering(const colValue_t &, const colValue_t &)>, struct CompareFunction>;

    enum class sortOrder_e : bool
    {
        ASC,
        DESC
    };

    struct sortPolicy_t
    {
        colIndex_t colIndex;   // Which column, relative to the table, should we sort on
        sortOrder_e sortOrder; // Should we sort in ascending order (If not, sort in descending order)
    };

    /**
     * @brief Generate a weak ordering value between two values of the same type T
     *
     * @tparam T    ONLY has < and == defined and does NOT have the spaceship operator
     * @return std::weak_ordering
     */
    template <has_less_equal_op T>
        requires(!has_weak_spaceship_op<T>)
    static inline auto longCompare(const T &left, const T &right) -> std::weak_ordering
    {
        if (left == right)
            return std::weak_ordering::equivalent;

        if (left < right)
            return std::weak_ordering::less;

        return std::weak_ordering::greater;
    }

    /**
     * @brief Compares two column values, using spaceship operator if possible
     *
     * @tparam T Underlying type in the variant
     * @return std::weak_ordering between values
     */
    template <ColumnType T>
    static auto compareType(const colValue_t &lhs, const colValue_t &rhs) -> std::weak_ordering
    {
        const auto left = std::get<T>(lhs);
        const auto right = std::get<T>(rhs);

        // This is the *clean* version but Apple clang doesn't currently implement this
        // https://en.cppreference.com/w/cpp/compiler_support/20#:~:text=19.29%20(16.10)*-,13.1.6*%20(partial),-constexpr%20default%20constructor
        // return std::std::compare_weak_order_fallback<T>(left, right);

        // Prefer the spaceship operator when we can, fallback to long compare otherwise
        if constexpr (has_weak_spaceship_op<T>)
            return left <=> right;
        else
            return longCompare(left, right);
    }

    /**
     * @brief Depending on the column type, return a different comparison function
     *
     *  NOTE: To save space, you *could* prevent duplicates of the same function in tables
     *        but that would increase complexity and this is already relatively cheap
     *
     * @param ct Column type
     * @return compareFunc_f How to compare two values associated with the given column type
     */
    static auto getTypeSpecificComparisonFunc(const colType_e ct) -> compareFunc_f
    {
        switch (ct)
        {
        case colType_e::INTEGER:
            return compareFunc_f(compareType<int>);
        case colType_e::STRING:
            return compareFunc_f(compareType<std::string>);
        case colType_e::DOUBLE:
            return compareFunc_f(compareType<double>);
        case colType_e::BOOLEAN:
            return compareFunc_f(compareType<bool>);
        case colType_e::COLOR:
            return compareFunc_f(compareType<Color>);
        }
    }

    /**
     * @brief In the case that this sort policy is DESC, flip the sign of the ordering
     */
    static auto flipOrdering(std::weak_ordering wo) -> std::weak_ordering
    {
        if (wo == std::weak_ordering::less)
            return std::weak_ordering::greater;

        if (wo == std::weak_ordering::equivalent)
            return std::weak_ordering::equivalent;

        return std::weak_ordering::less;
    }

    /**
     * @brief C++ mandates an actual object has to exist so this is that
     *        Useful because it can save and modify it's state
     */
    struct sortHelper_t
    {
        sortHelper_t(std::vector<policyFunc_f> &&funcs)
            : sortPriorityFunctions(std::move(funcs)) {}

        /**
         * @brief This is what will end up getting called when std::sort has to decide which
         *        row to place first
         */
        auto operator()(const row_t &lhs, const row_t &rhs) -> bool
        {
            // Go through as many functions as we have before we find a non-equivalent weak ordering
            for (const auto &compareFunc : sortPriorityFunctions)
            {
                auto compareResult = compareFunc.get()(lhs, rhs);

                // Need to use next policy to break the tie
                if (compareResult == std::weak_ordering::equivalent)
                    continue;

                return compareResult == std::weak_ordering::less;
            }

            // Getting here is a complete tie, so as a default, lhs is NOT less than rhs
            return false;
        };

    private:
        std::vector<policyFunc_f> sortPriorityFunctions; // In order, how we should evaluate a row against another row
    };
}

#endif // INCLUDE_SORT_HELPERS_H
