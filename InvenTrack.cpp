#include <iostream>
#include <map>
#include <algorithm>
#include <cstdlib>
#include "scanin.cpp"
#include "scanout.cpp"

int main(int argc, char* argv[]) {
    std::map<std::string, std::function<void()>> commands = {
            {"scanin", &InvenTrack::scanin},
            {"scanout", &InvenTrack::scanout}

    };

    if (argc < 2) { // There is no command
        std::cerr << "Error: You forgot to put the mode" << '\n';
        std::cerr << "Usage: InvenTrack <mode> <...>" << '\n';
    }

    std::string input {argv[1]};

    auto function = commands.find(input);
    if (function != commands.end()) {
        function->second();
    }

//    std::for_each(
//            begin(commands), end(commands),
//            [input](auto const& command) {
//                if (command == input) {
//                    std::cout << "Found" << '\n' << "Now Running" << "\n";
//                    std::string runCommand {"./" + command};
//                    std::system(runCommand.c_str());
//                } else {
//                    std::cout << "Not Found" << '\n';
//                }
//            });

}