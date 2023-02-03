#ifndef TABLE_TABLE_HELPERS_H
#define TABLE_TABLE_HELPERS_H

#include <iostream>
#include <variant>
#include <vector>

namespace table
{
    /**
     * @brief All column types have to meet these requirements
     */
    template <typename T>
    concept ColumnType = requires(T a, T b) {
                             // By convention, all you need to sort a container is this operation defined
                             a < b;

                             // Have some sort of way to print it (Soft requirement)
                             std::cout << a;
                         };

    /**
     * @brief Define all the available column types available to use
     *
     *  NOTE: Should match up 1:1 with types within colValue_t variant (Helps with checking validity)
     */
    enum class colType_e : uint8_t
    {
        INTEGER = 0,
        STRING = 1,
        BOOLEAN = 2,
        DOUBLE = 3,
    };

    using colValue_t = std::variant<int, std::string, bool, double>;

    // For simplicity, just assume a row is a vector of column values. This can get a lot more complex
    using row_t = std::vector<const colValue_t>;
    using rows_t = std::vector<row_t>;

    /**
     * @brief Prints out every value in a row to std::cout
     */
    static inline void printRow(const row_t &row)
    {
        auto print = [](const auto &val)
        {
            std::cout << val << " ";
        };

        for (const auto &val : row)
        {
            std::visit(print, val);
        }
        std::cout << '\n';
    }
}

#endif // TABLE_TABLE_HELPERS_H