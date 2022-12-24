#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sqlite3.h>

// A function to print table from SQLite Database in a visually appealing way
void printTable(sqlite3* db, const std::string& tableName) {
    // Made SELECT statement
    std::string statement = "SELECT * FROM " + tableName;

    // Compile SELECT statement into a prepared statement
    sqlite3_stmt* preparedStatement;
    int rc = sqlite3_prepare_v2(db, statement.c_str(), -1, &preparedStatement, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Determine number of columns in table
    int numOfColumns = sqlite3_column_count(preparedStatement);

    // Retreive column names and store in vector
    std::vector<std::string> columnNames;
    for (int i = 0; i < numOfColumns; i ++) {
        columnNames.push_back(sqlite3_column_name(preparedStatement, i));
    }
    
    // Find the maximum column width
    std::vector<int> columnWidths(numOfColumns);
    while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
        for (int i = 0; i < numOfColumns; i++) {
            int width = std::string(reinterpret_cast<const char*>(sqlite3_column_text(preparedStatement, i))).size();
            columnWidths[i] = std::max(columnWidths[i], width);
        }
    }

    // Reset the prepared statement for reuse
    sqlite3_reset(preparedStatement);

    // Print the table header
    for (int i = 0; i < numOfColumns; i ++) {
        std::cout << std::left << std::setw(columnWidths[i]) << columnNames[i] << "  ";
    }
    std::cout << std::endl;

    // Print a separator line
    for (int i = 0; i < numOfColumns; i ++) {
        std::cout << std::string(columnWidths[i], '-') << "  ";
    }
    std::cout << std::endl;

    // Print the table rows
    while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
        for (int i = 0; i < numOfColumns; i ++) {
            std::cout << std::left << std::setw(columnWidths[i]) << std::string(reinterpret_cast<const char*>(sqlite3_column_text(preparedStatement, i))) << "  ";
        }
        std::cout << std::endl;
    }
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("Inventory.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening SQLite Database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    printTable(db, "Product");

    sqlite3_close(db);
    return EXIT_SUCCESS;
}