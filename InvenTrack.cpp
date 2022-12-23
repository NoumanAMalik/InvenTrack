#include <iostream>
#include <vector>
#include <algorithm>
#include <sqlite3.h>

int main() {
    sqlite3* db;
    int rc = sqlite3_open("Inventory.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening SQLite Database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    sqlite3_close(db);
    return EXIT_SUCCESS;
}