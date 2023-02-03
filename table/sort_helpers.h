#ifndef TABLE_SORT_HELPERS_H
#define TABLE_SORT_HELPERS_H

#include <functional>

#include "generic/named_type.h"
#include "table_helpers.h"

namespace table
{
    /**
     * @brief Used by the comparison function to relay relative ordering between two "things"
     *        At least 3 values are needed
     */
    enum class compareResult_e : int8_t
    {
        LESS_THAN = -1, // By setting LESS_THAN and GREATER_THAN to signed opposites, makes it easy to flip
        EQUAL_TO = 0,
        GREATER_THAN = 1
    };

    /**
     * @brief Try to use strong types as much as possible over primitive types
     */
    using colIndex_t = NamedType<int, struct columnIndex>;
    enum class sortOrder_e : bool
    {
        ASC,
        DESC
    };

    /**
     * @brief What we need to know to sort
     */
    struct sortPolicy_t
    {
        colIndex_t colIndex;   // Which column, relative to the table, should we sort on
        sortOrder_e sortOrder; // Should we sort in ascending order (If not, sort in descending order)
    };

    using policyFunc_f = NamedType<std::function<compareResult_e(const row_t &, const row_t &)>, struct PolicyFunction>;
    using compareFunc_f = NamedType<std::function<compareResult_e(const colValue_t &, const colValue_t &)>, struct CompareFunction>;

    /**
     * @brief Compares two column values and returns the ordering between the two
     *
     * @tparam T Underlying type in the variant
     * @param lhs
     * @param rhs
     * @return compareResult_e
     */
    template <ColumnType T>
    static compareResult_e compareType(const colValue_t &lhs, const colValue_t &rhs)
    {
        const auto left = std::get<T>(lhs);
        const auto right = std::get<T>(rhs);

        // There are other ways to do this, but in theory, the < operator is all that's needed
        if (left < right)
        {
            return compareResult_e::LESS_THAN;
        }

        if (right < left)
        {
            return compareResult_e::GREATER_THAN;
        }

        return compareResult_e::EQUAL_TO;
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
    static compareFunc_f getTypeSpecificComparisonFunc(const colType_e ct)
    {
        switch (ct)
        {
        case colType_e::INTEGER:
        {
            return compareFunc_f(compareType<int>);
        }
        case colType_e::STRING:
        {
            return compareFunc_f(compareType<std::string>);
        }
        case colType_e::DOUBLE:
        {
            return compareFunc_f(compareType<double>);
        }
        case colType_e::BOOLEAN:
        {
            return compareFunc_f(compareType<bool>);
        }
        }
    }

    /**
     * @brief C++ mandates an actual object has to exist so this is that
     *        Useful because it can save and modify it's state
     */
    struct sortHelper_t
    {
        bool operator()(const row_t &lhs, const row_t &rhs)
        {
            for (const auto &f : sortPriorityFunctions)
            {
                // Compare the value for this sort priority
                auto compareResult = f.get()(lhs, rhs);

                // Need to use next tie breaker
                if (compareResult == compareResult_e::EQUAL_TO)
                {
                    continue;
                }

                return compareResult == compareResult_e::LESS_THAN;
            }

            // Getting here is a complete tie, so by std::sort() convention, lhs is
            return false;
        };

        std::vector<const policyFunc_f> sortPriorityFunctions; // In order, how we should evaluate a row against another row
    };
}

#endif // TABLE_SORT_HELPERS_H
