#ifndef INCLUDE_TABLE_H
#define INCLUDE_TABLE_H

#include <assert.h>
#include <functional>
#include <iterator>

#include "table_helpers.h"
#include "sort_helpers.h"

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
        template <typename C = std::vector<colType_e>>
            requires std::is_constructible_v<std::vector<colType_e>, C>
        table_t(C &&tableDef)
            : m_tableDef(std::forward<C>(tableDef)),
              m_compareFuncs(),
              m_rows()
        {
            // ASSUMPTION: Table definition is constant
            // Pre-define the comparison for each column since each column type is known
            m_compareFuncs.reserve(m_tableDef.size());
            for (const auto colType : m_tableDef)
                m_compareFuncs.emplace_back(getTypeSpecificComparisonFunc(colType));
        }

        auto appendRow(row_t &&row) -> void
        {
            assert(isRowValid(row));
            m_rows.emplace_back(std::move(row));
        }

        /**
         * @brief Sorts the table based on the passed in policies
         *
         *        The whole gimmick here is to have the STL sort for us with std::sort
         *        And we just create the sorting operator
         *
         * @param sortPolicies
         */
        template <typename C = std::vector<sortPolicy_t>>
        auto sort(const C &sortPolicies) -> void
        {
            std::vector<policyFunc_f> sortPriorityFunctions;
            sortPriorityFunctions.reserve(sortPolicies.size());

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
                        return flipOrdering(compareRes);

                    return compareRes;
                };

                sortPriorityFunctions.emplace_back(std::move(compareFunc));
            }

            // Once we have all our policy functions built, use the STL sort
            std::sort(m_rows.begin(), m_rows.end(), sortHelper_t(std::move(sortPriorityFunctions)));
        }

        auto getRows() noexcept const -> const rows_t & { return m_rows; }
        auto getTableDef() noexcept const -> const std::vector<colType_e> & { return m_tableDef; }

    private:
        inline auto isRowValid(const row_t &row) noexcept -> bool
        {
            // Correct # of elements
            if (row.size() != m_tableDef.size())
                return false;

            // Each type matches respective column type
            for (auto i = 0; i < m_tableDef.size(); ++i)
            {
                // A little bit of a hack based on setting the enums
                if (row[i].index() != static_cast<int>(m_tableDef[i]))
                    return false;
            }

            return true;
        }

        inline auto isSortPolicyValid(const sortPolicy_t &sp) noexcept -> bool
        {
            // Valid column index
            return sp.colIndex.get() < m_tableDef.size();
        }

        std::vector<colType_e> m_tableDef;         // Represents the columns and their types
        std::vector<compareFunc_f> m_compareFuncs; // Parallel to m_tableDef, for each column, what compare func should it use

        rows_t m_rows; // Holds the rows of the table
    };
}

#endif // INCLUDE_TABLE_H