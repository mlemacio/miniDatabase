#include <gtest/gtest.h>
#include <memory>

#include "include/table.h"

namespace test
{
    using namespace table;

    auto setUpTable(auto &&tableDef, auto &&rowsBefore) -> std::unique_ptr<table_t>
    {
        auto t = std::make_unique<table_t>(std::move(tableDef));
        for (auto &row : rowsBefore)
        {
            t->appendRow(std::move(row));
        }

        return t;
    }

    void runTest(const auto &tPtr,
                 const auto &rowsAfter,
                 auto &&sorts)
    {
        tPtr->sort(std::move(sorts));
        auto rowsAfterSort = tPtr->getRows();

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

        auto tPtr = setUpTable(std::move(tableDef), std::move(rowsBefore));

        rows_t rowsAfter = {
            {"aa", 3, true},
            {"aa", 4, true},
            {"bbb", 1, true},
            {"bbb", 0, false},
            {"xx", 2, false},
        };
        std::vector<sortPolicy_t> sortPolicies = {
            {colIndex_t(0), sortOrder_e::ASC},
        };
        runTest(tPtr, std::move(rowsAfter), std::move(sortPolicies));
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

        auto tPtr = setUpTable(std::move(tableDef), std::move(rowsBefore));

        rows_t rowsAfter = {
            {"bbb", 0, false},
            {"xx", 2, false},
            {"bbb", 1, true},
            {"aa", 3, true},
            {"aa", 4, true},
        };
        std::vector<sortPolicy_t> sortPolicies = {
            {colIndex_t(2), sortOrder_e::ASC},
            {colIndex_t(1), sortOrder_e::ASC}};

        runTest(tPtr, std::move(rowsAfter), std::move(sortPolicies));
    }

    TEST(tableTest, multipleSorts)
    {
        // Define our table
        std::vector<colType_e> tableDef = {colType_e::STRING, colType_e::INTEGER, colType_e::BOOLEAN};
        rows_t rowsBefore = {
            {"aa", 3, true},
            {"bbb", 1, true},
            {"xx", 2, false},
            {"aa", 4, true},
            {"bbb", 0, false},
        };

        auto tPtr = setUpTable(std::move(tableDef), std::move(rowsBefore));

        rows_t rowsAfter = rows_t{
            {"aa", 3, true},
            {"aa", 4, true},
            {"bbb", 1, true},
            {"bbb", 0, false},
            {"xx", 2, false},
        };

        std::vector<sortPolicy_t> sortPolicies = {
            sortPolicy_t{colIndex_t(0), sortOrder_e::ASC},
        };

        runTest(tPtr, std::move(rowsAfter), std::move(sortPolicies));

        rows_t rowsAfter_1 = rows_t{
            {"bbb", 0, false},
            {"xx", 2, false},
            {"bbb", 1, true},
            {"aa", 3, true},
            {"aa", 4, true},
        };

        std::vector<sortPolicy_t> sortPolicies_1 = {
            {colIndex_t(2), sortOrder_e::ASC},
            {colIndex_t(1), sortOrder_e::ASC},
        };
        runTest(tPtr, std::move(rowsAfter_1), std::move(sortPolicies_1));

        rows_t rowsAfter_2 = {
            {"aa", 4, true},
            {"aa", 3, true},
            {"xx", 2, false},
            {"bbb", 1, true},
            {"bbb", 0, false},
        };

        std::vector<sortPolicy_t> sortPolicies_2 = {
            {colIndex_t(1), sortOrder_e::DESC},
            {colIndex_t(2), sortOrder_e::ASC}};
        runTest(tPtr, std::move(rowsAfter_2), std::move(sortPolicies_2));
    }
};