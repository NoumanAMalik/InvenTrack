#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    std::vector<std::string> commands{"scanin", "scanout"};

    if (argc < 2) { // There is no command
        std::cerr << "Error: You forgot to put the mode" << '\n';
        std::cerr << "Usage: InvenTrack <mode> <...>" << '\n';
    }

    std::string input {argv[1]};

    std::for_each(
            begin(commands), end(commands),
            [input](auto const& command) {
                if (command == input) {
                    std::cout << "Found" << '\n';
                } else {
                    std::cout << "Not Found" << '\n';
                }
            });

}