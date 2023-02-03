#include <iostream>

#include "table/table.h"

using namespace table;

int main()
{
    std::cout << std::boolalpha;

    // Define our table
    table_t t({colType_e::STRING, colType_e::INTEGER, colType_e::BOOLEAN});

    // Add rows to our table
    t.appendRow({"aa", 3, true});
    t.appendRow({"bbb", 1, true});
    t.appendRow({"xx", 2, false});
    t.appendRow({"aa", 4, true});
    t.appendRow({"bbb", 0, false});

    std::cout << "Starting Table" << '\n';
    t.print();
    std::cout << '\n';

    // Sort in various ways and print it
    std::cout << "Table after Sort Ordering: {{0, asc}}" << '\n';
    t.sort({{colIndex_t(0), sortOrder_e::ASC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{2, asc}, {1, asc}}" << '\n';
    t.sort({{colIndex_t(2), sortOrder_e::ASC}, {colIndex_t(1), sortOrder_e::ASC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{2, asc}, {1, desc}}" << '\n';
    t.sort({{colIndex_t(2), sortOrder_e::ASC}, {colIndex_t(2), sortOrder_e::DESC}});
    t.print();
    std::cout << '\n';

    std::cout << "Table after Sort Ordering: {{1, desc}, {2, asc}}" << '\n';
    t.sort({{colIndex_t(2), sortOrder_e::DESC}, {colIndex_t(2), sortOrder_e::ASC}});
    t.print();
    std::cout << '\n';
    return 0;
}