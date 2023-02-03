#include <gtest/gtest.h>

#include "table/table.h"

namespace test
{
    using namespace table;

    table_t setUpTable(std::vector<colType_e> &&tableDef, rows_t &&rowsBefore)
    {
        table_t t(std::move(tableDef));
        for (auto &row : rowsBefore)
        {
            t.appendRow(std::move(row));
        }

        return t;
    }

    void runTest(table_t &t,
                 rows_t &&rowsAfter,
                 std::vector<sortPolicy_t> &&sorts)
    {
        t.sort(std::move(sorts));
        auto rowsAfterSort = t.getRows();

        EXPECT_EQ(rowsAfterSort.size(), rowsAfter.size());

        for (auto i = 0; i < rowsAfterSort.size(); ++i)
        {
            EXPECT_EQ(rowsAfterSort[i].size(), rowsAfter[i].size());
            for (auto j = 0; i < rowsAfterSort.size(); ++i)
            {
                EXPECT_EQ(rowsAfterSort[i][j], rowsAfter[i][j]);
            }
        }
    }

    /**
     * @brief There's not *super* much to test here since the actual sort is handled by the STL
     *        and all of the data types of trivial
     */
    TEST(tableTest, basicSort_1)
    {
        // Define our table
        auto tableDef = {colType_e::STRING, colType_e::INTEGER, colType_e::BOOLEAN};
        rows_t rowsBefore = {
            {"aa", 3, true},
            {"bbb", 1, true},
            {"xx", 2, false},
            {"aa", 4, true},
            {"bbb", 0, false},
        };

        table_t t = setUpTable(std::move(tableDef), std::move(rowsBefore));

        rows_t rowsAfter = {
            {"aa", 3, true},
            {"aa", 4, true},
            {"bbb", 1, true},
            {"bbb", 0, false},
            {"xx", 2, false},
        };
        std::vector<sortPolicy_t> sortPolicies = {
            {0, true},
        };
        runTest(t, std::move(rowsAfter), std::move(sortPolicies));
    }

    TEST(tableTest, basicSort_2)
    {
        // Define our table
        auto tableDef = {colType_e::STRING, colType_e::INTEGER, colType_e::BOOLEAN};
        rows_t rowsBefore = {
            {"aa", 3, true},
            {"bbb", 1, true},
            {"xx", 2, false},
            {"aa", 4, true},
            {"bbb", 0, false},
        };

        table_t t = setUpTable(std::move(tableDef), std::move(rowsBefore));

        rows_t rowsAfter = {
            {"bbb", 0, false},
            {"xx", 2, false},
            {"bbb", 1, true},
            {"aa", 3, true},
            {"aa", 4, true},
        };
        std::vector<sortPolicy_t> sortPolicies = {
            {2, true},
            {1, true},
        };

        runTest(t, std::move(rowsAfter), std::move(sortPolicies));
    }

    TEST(tableTest, multipleSorts)
    {
        // Define our table
        auto tableDef = {colType_e::STRING, colType_e::INTEGER, colType_e::BOOLEAN};
        rows_t rowsBefore = {
            {"aa", 3, true},
            {"bbb", 1, true},
            {"xx", 2, false},
            {"aa", 4, true},
            {"bbb", 0, false},
        };

        table_t t = setUpTable(std::move(tableDef), std::move(rowsBefore));

        rows_t rowsAfter = {
            {"aa", 3, true},
            {"aa", 4, true},
            {"bbb", 1, true},
            {"bbb", 0, false},
            {"xx", 2, false},
        };
        std::vector<sortPolicy_t> sortPolicies = {
            {0, true},
        };
        runTest(t, std::move(rowsAfter), std::move(sortPolicies));

        rowsAfter = {
            {"bbb", 0, false},
            {"xx", 2, false},
            {"bbb", 1, true},
            {"aa", 3, true},
            {"aa", 4, true},
        };
        sortPolicies = {
            {2, true},
            {1, true},
        };
        runTest(t, std::move(rowsAfter), std::move(sortPolicies));

        rowsAfter = {
            {"aa", 4, true},
            {"aa", 3, true},
            {"xx", 2, false},
            {"bbb", 1, true},
            {"bbb", 0, false},
        };
        sortPolicies = {
            {1, false},
            {2, true},
        };
        runTest(t, std::move(rowsAfter), std::move(sortPolicies));
    }
};