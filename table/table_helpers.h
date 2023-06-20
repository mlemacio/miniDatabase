#ifndef INCLUDE_TABLE_HELPERS_H
#define INCLUDE_TABLE_HELPERS_H

#include <iostream>
#include <variant>
#include <vector>

#include "generic/color.h"

namespace table
{
    template <typename T>
    concept has_weak_spaceship_op = requires(T a, T b) {
        {
            a <=> b
        } -> std::convertible_to<std::weak_ordering>;
    };

    template <typename T>
    concept has_less_equal_op = requires(T a, T b) {
        {
            a < b
        } -> std::same_as<bool>;
        {
            a == b
        } -> std::same_as<bool>;
    };

    template <typename T>
    concept is_printable = requires(T a) {
        std::cout << a;
    };

    template <typename T>
    concept is_weakly_orderable = has_weak_spaceship_op<T> || has_less_equal_op<T>;

    /**
     * @brief All column types have to meet these requirements
     */
    template <typename T>
    concept ColumnType = is_weakly_orderable<T> && is_printable<T>;

    /**
     * @brief Define all the available column types available to use
     *
     *  NOTE: Should match up 1:1 with types within colValue_t variant
     */
    enum class colType_e : uint8_t
    {
        INTEGER = 0,
        STRING = 1,
        BOOLEAN = 2,
        DOUBLE = 3,
        COLOR = 4,
    };

    using colValue_t = std::variant<int, std::string, bool, double, Color>;
    using row_t = std::vector<colValue_t>;
    using rows_t = std::vector<row_t>;

    /**
     * @brief Prints out every value in a row to std::cout
     */
    static inline auto printRow(const row_t &row) -> void
    {
        static auto print = [](const auto &val)
        {
            std::cout << val << " ";
        };

        for (const auto &val : row)
            std::visit(print, val);

        std::cout << '\n';
    }

    static inline auto printRows(const rows_t &rows) -> void
    {
        for (const auto &row : rows)
            printRow(row);
    }

    static inline auto colTypeToString(colType_e ct) noexcept -> std::string_view
    {
        switch (ct)
        {
        case colType_e::INTEGER:
            return "INTEGER";
        case colType_e::STRING:
            return "STRING";
        case colType_e::BOOLEAN:
            return "BOOLEAN";
        case colType_e::DOUBLE:
            return "DOUBLE";
        case colType_e::COLOR:
            return "COLOR";
        }
    }

    static inline auto printTableDef(const std::vector<colType_e> &tableDef)
    {
        for (const auto &colType : tableDef)
        {
            std::cout << colTypeToString(colType) << " ";
        }
        std::cout << '\n';
    }
}

#endif // INCLUDE_TABLE_HELPERS_H