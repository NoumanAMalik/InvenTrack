#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sqlite3.h>
#include <chrono>
#include <string>
#include <iterator>
#include <unistd.h>

// Benchmark function to test time
template <typename Func>
auto benchmark(Func f) {
    auto startTime = std::chrono::steady_clock::now();

    f();

    auto endTime = std::chrono::steady_clock::now();

    double time = std::chrono::duration<double>(endTime - startTime).count();

    std::cout << "benchmark() took " << time << " seconds" << std::endl;
}

// Function to simplify turning a string to lowercase
auto toLower(std::string& s) {
    std::transform(
        s.begin(), s.end(),
        s.begin(),
        [](char c) {
            return std::tolower(c);
        }
    );
}

// Gets the ProductId field from the Product Table
// Parameters: sqlite database, and upc number
// Returns ProductId
auto getProductId(sqlite3* db, std::string const& upc) -> std::string {
    std::string statement = "SELECT Id FROM Product WHERE UPC = " + upc + ";";

    sqlite3_stmt* preparedStatement;
    int rc = sqlite3_prepare_v2(db, statement.c_str(), -1, &preparedStatement, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        return "NOT OK";
    }

    rc = sqlite3_step(preparedStatement);
    if (rc == SQLITE_ROW) {
        std::string productId = std::string(reinterpret_cast<const char*>(sqlite3_column_text(preparedStatement, 0)));
        return productId;
    } else {
        std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        return "NO ROW";
    }
}

auto scanIn(sqlite3* db, std::string const& upc, std::string const& quantity) -> int {
    std::string statement = "INSERT INTO Inventory(ProductId, Quantity) "
                            "SELECT Id, " + quantity + " FROM Product "
                            "WHERE Product.UPC = " + upc + ";";

    sqlite3_stmt* preparedStatement;
    int rc = sqlite3_prepare_v2(db, statement.c_str(), -1, &preparedStatement, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    rc = sqlite3_step(preparedStatement);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    return 1;
}

auto clearScreen() {
    // Clear Screen
    std::cout << "\033[2J"; // Clears Screen
    std::cout << "\033[H"; // Moves cursor to position (1, 1) [The top left of the screen]
}

// A function to print table from SQLite Database in a visually appealing way
void printTable(sqlite3* db, const std::string& tableName) {
    clearScreen();

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

    // Retrieve column names and store in vector
    std::vector<std::string> columnNames;
    std::vector<int> columnWidths(numOfColumns);

    for (int i = 0; i < numOfColumns; i ++) {
        std::string columnName = sqlite3_column_name(preparedStatement, i);
        int width = static_cast<int>(columnName.size());
        columnNames.push_back(columnName);
        columnWidths[i] = std::max(columnWidths[i], width);
    }
    
    // Find the maximum column width
    while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
        for (int i = 0; i < numOfColumns; i++) {
            int width = static_cast<int>(std::string(reinterpret_cast<const char*>(sqlite3_column_text(preparedStatement, i))).size());
            columnWidths[i] = std::max(columnWidths[i], width);
        }
    }

    // Reset the prepared statement for reuse
    sqlite3_reset(preparedStatement);

	// // Print a separator line
 //    for (int i = 0; i < numOfColumns; i ++) {
	// 	if (i == 0) {
	// 		std::cout << "| ";
	// 	}
 //        std::cout << std::string(columnWidths[i], '-') << " | ";
 //    }
 //    std::cout << std::endl;
	
    // Print the table header
    for (int i = 0; i < numOfColumns; i ++) {
		if (i == 0) {
			std::cout << "| ";
		}
        std::cout << std::left << std::setw(columnWidths[i]) << columnNames[i] << " | ";
    }
    std::cout << std::endl;

    // Print a separator line
    for (int i = 0; i < numOfColumns; i ++) {
		if (i == 0) {
			std::cout << "| ";
		}
        std::cout << std::string(columnWidths[i], '-') << " | ";
    }
    std::cout << std::endl;

    // Print the table rows
    while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
        for (int i = 0; i < numOfColumns; i ++) {
			if (i == 0) {
				std::cout << "| ";
			}
            std::cout << std::left << std::setw(columnWidths[i]) << std::string(reinterpret_cast<const char*>(sqlite3_column_text(preparedStatement, i))) << " | ";
        }
        std::cout << std::endl;
        // Print a separator line
        for (int i = 0; i < numOfColumns; i ++) {
			if (i == 0) {
				std::cout << "| ";
			}
            std::cout << std::string(columnWidths[i], '-') << " | ";
        }
        std::cout << std::endl;
    }
}

auto setup(const char* databaseName) -> sqlite3* {
    sqlite3* db;
    int rc = sqlite3_open(databaseName, &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening SQLite Database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    return db;
}

int main(int argc, char* argv[]) {
    std::string modeInput {};
    std::string upcInput {};
    std::string quantityInput {};
    int opt;

    // Get flags
    while ((opt = getopt(argc, argv, "m:u:q:")) != -1) {
        switch (opt) {
            case 'm':
                modeInput = optarg;
                break;
            case 'u':
                upcInput = optarg;
                break;
            case 'q':
                quantityInput = optarg;
                break;
            default:
                std::cerr << "Invalid option: " << opt << std::endl;
                return EXIT_FAILURE;
        }
    }

    std::string scanin {"scanin"};
	std::string addproduct {"addproduct"};
 
    toLower(modeInput);
    
    auto db = setup("Inventory.db");

    if (scanin.compare(modeInput) == 0) {
        std::cout << scanIn(db, upcInput, quantityInput) << std::endl;
    } else if (addproduct.compare(modeInput) == 0) {
		// Enter Code Here For Adding a Product
        std::cout << "Please enter the following information in the following order: Name, UPC, Description, Price." << std::endl;
        std::cout << "If Name or Description need more than one word, surround with quotes" << std::endl;
        std::cout << "When you are done, enter the word \"done\" or hit CRTL+D" << std::endl;

        std::string name;
        std::string upc;
        std::string description;
        std::string price;

        std::list<std::string> input {};
        std::string cur_input {};

        while (std::getline(std::cin, cur_input)) {
            if (cur_input == "done") {
                std::cout << "You are printing \n";
                break;
            }
            std::cin.clear();
            input.push_back(cur_input);
        }

        if (input.size() == 4) {
            name = input.front();
            input.pop_front();
        }

        std::copy(
                input.begin(), input.end(),
                std::ostream_iterator<std::string>(std::cout, ", ")
        );

		// Get Name
		// Get UPC
		// Get Description
		// Get Price
		// Allow the user to keep entering the information in order until they enter the word done, or they exit the current document
	}



    sqlite3_close(db);
    return EXIT_SUCCESS;
}