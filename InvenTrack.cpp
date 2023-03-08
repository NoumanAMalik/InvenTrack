#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    std::vector<std::string> commands {"scanin", "scanout"};

    if (argc < 2) { // There is no command
        std::cerr << "Error: You forgot to put the mode" << '\n';
        std::cerr << "Usage: InvenTrack <mode> <...>" << '\n';
    }

    auto foundCommand = std::find(commands.begin(), commands.end(), argv[1]);
    
    if (*foundCommand != "") std::cout << *foundCommand << '\n';
    else std::cout << "Not a command" << '\n';

    std::cout << "\n";
}