#include <iostream>

#include <deque>
#include <list>
#include <vector>

#include "table/table.h"

using namespace table;

int main()
{
    std::cout << std::boolalpha;

    // Define our table
    table_t t({colType_e::STRING, colType_e::INTEGER, colType_e::BOOLEAN, colType_e::COLOR});

    // Add rows to our table
    t.appendRow({"aa", 3, true, Color::color_e::RED});
    t.appendRow({"bbb", 1, true, Color::color_e::GREEN});
    t.appendRow({"xx", 2, false, Color::color_e::BLUE});
    t.appendRow({"aa", 4, true, Color::color_e::RED});
    t.appendRow({"bbb", 0, false, Color::color_e::RED});

    std::cout << "Starting Table" << '\n';
    t.print();
    std::cout << '\n';

    // Sort in various ways and print it
    std::cout << "Table after Sort Ordering: {{0, asc}}" << '\n';
    t.sort(std::vector<sortPolicy_t>{{colIndex_t(0), sortOrder_e::ASC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{2, asc}, {1, asc}}" << '\n';
    t.sort(std::list<sortPolicy_t>{{colIndex_t(2), sortOrder_e::ASC}, {colIndex_t(1), sortOrder_e::ASC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{2, asc}, {1, desc}}" << '\n';
    t.sort(std::deque<sortPolicy_t>{{colIndex_t(2), sortOrder_e::ASC}, {colIndex_t(2), sortOrder_e::DESC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{1, desc}, {2, asc}}" << '\n';
    t.sort({{colIndex_t(2), sortOrder_e::DESC}, {colIndex_t(2), sortOrder_e::ASC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{3, desc}}" << '\n';
    t.sort({{colIndex_t(3), sortOrder_e::DESC}});
    t.print();
    std::cout << '\n';
    return 0;
}