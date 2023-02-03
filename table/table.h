#ifndef TABLE_TABLE_H
#define TABLE_TABLE_H

#include <assert.h>
#include <functional>
#include <iterator>
#include <ranges>

#include "table_helpers.h"
#include "sort_helpers.h"
#include "type.h"

namespace table
{
    /**
     * @brief Internal reprentation of a table in a database
     *
     * ASSUMPTION: The table defintion doesn't change. While not true in reality, we assume for the example
     *
     *       NOTE: Because of the nature of how we solve the problem of "sorting rows in a database", this is all
     *             just an overglorified wrapped around std::sort so past some basics, there isn't much to test
     */
    class table_t
    {
    public:
        /**
         * @brief Construct a new table_t object
         *
         * @param tableDef Ordered container of types
         */
        template <ContainerOf<const colType_e> C = std::vector<const colType_e>>
        table_t(C &&tableDef)
            : m_tableDef(std::move(tableDef)),
              m_compareFuncs(),
              m_rows()
        {
            // ASSUMPTION: Table definition is constant
            // Pre-define the comparison for each column since each column type is known
            m_compareFuncs.reserve(m_tableDef.size());
            for (const auto colType : m_tableDef)
            {
                m_compareFuncs.emplace_back(getTypeSpecificComparisonFunc(colType));
            }
        }

        /**
         * @brief Add a new row to the current table
         */
        void appendRow(row_t &&row)
        {
            assert(isRowValid(row));
            m_rows.emplace_back(std::move(row));
        }

        /**
         * @brief Sorts the table based on the passed in policies
         *
         * @param sortPolicies
         */
        template <ContainerOf<const sortPolicy_t> C = std::vector<const sortPolicy_t>>
        void sort(const C &sortPolicies)
        {
            // The whole gimmick here is to have the STL sort for us with std::sort
            // And we just create the sorting operator
            sortHelper_t sh;
            sh.sortPriorityFunctions.reserve(sortPolicies.size());

            // For every policy, build a comparison function between two rows for this sort policy
            for (const auto &policy : sortPolicies)
            {
                assert(isSortPolicyValid(policy));

                auto compareFunc = [this, &policy](const row_t &lhs, const row_t &rhs)
                {
                    auto &leftVal = lhs[policy.colIndex.get()];
                    auto &rightVal = rhs[policy.colIndex.get()];

                    // Grab the comparison function based on which column we're dealing with and make comparison
                    auto &valCompareFunc = m_compareFuncs[policy.colIndex.get()];
                    auto compareRes = valCompareFunc.get()(leftVal, rightVal);

                    if (policy.sortOrder == sortOrder_e::DESC)
                    {
                        // This trick only works when you set the values in the enum
                        compareRes = static_cast<compareResult_e>(-1 * static_cast<int>(compareRes));
                    }

                    return compareRes;
                };

                sh.sortPriorityFunctions.emplace_back(std::move(compareFunc));
            }

            // Once we have all our policy functions built, use the STL sort
            std::sort(m_rows.begin(), m_rows.end(), sh);
        }

        /**
         * @brief Prints out the table to cout
         */
        void print() const
        {
            std::cout << "-------------------\n";
            for (const auto &row : m_rows)
            {
                printRow(row);
            }
        }

        /**
         * @brief Get the underlying rowss
         *
         *  NOTE: This can result in a dangling reference
         *
         * @return const std::vector<row_t>&
         */
        const rows_t &getRows() const
        {
            return m_rows;
        }

    private:
        /**
         * @brief Makes sure a passed in row matches the table definiion
         *
         *  NOTE: This can get a lot more nuanced depending on internal layout, we kind of just cheat a little
         *
         * @param row
         */
        bool isRowValid(const row_t &row)
        {
            if (row.size() != m_tableDef.size())
            {
                return false;
            }

            for (auto i = 0; i < m_tableDef.size(); ++i)
            {
                // A little bit of a hack based on setting the enums
                if (row[i].index() != static_cast<int>(m_tableDef[i]))
                {
                    return false;
                }
            }

            return true;
        }

        /**
         * @brief Checks a passed in policy is valid to sort on
         */
        bool isSortPolicyValid(const sortPolicy_t &sp)
        {
            if (sp.colIndex.get() >= m_tableDef.size())
            {
                return false;
            }

            return true;
        }

        std::vector<const colType_e> m_tableDef;         // Represents the columns and their types
        std::vector<const compareFunc_f> m_compareFuncs; // Parallel to m_tableDef, for each column, what compare func should it use

        rows_t m_rows; // Holds the rows of the table
    };
}

#endif // TABLE_TABLE_H