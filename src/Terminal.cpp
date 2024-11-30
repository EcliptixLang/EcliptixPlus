/*#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class ArgumentParser {
public:
    ArgumentParser(int argc, char* argv[]) {
        parseArguments(argc, argv);
    }

    bool hasFlag(const std::string& flag) const {
        return flags.find(flag) != flags.end();
    }

    std::string getFlagValue(const std::string& flag) const {
        auto it = flags.find(flag);
        if (it != flags.end()) {
            return it->second;
        }
        return "";
    }

    std::vector<std::string> getPositionalArgs() const {
        return positionalArgs;
    }

private:
    std::unordered_map<std::string, std::string> flags;  // Stores flags and their values
    std::vector<std::string> positionalArgs;  // Stores positional arguments

    void parseArguments(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg[0] == '-') {  // It's a flag
                if (i + 1 < argc && argv[i + 1][0] != '-') {
                    flags[arg] = argv[i + 1];  // Flag with value
                    ++i;  // Skip next argument as it's the value of the flag
                } else {
                    flags[arg] = "";  // Flag without value
                }
            } else {  // Positional argument (not a flag)
                positionalArgs.push_back(arg);
            }
        }
    }
};

int main(int argc, char* argv[]) {
    ArgumentParser parser(argc, argv);

    // Check if specific flags exist
    if (parser.hasFlag("-r")) {
        std::cout << "Flag -r is present." << std::endl;
    }

    // Get value for -l flag
    if (parser.hasFlag("-l")) {
        std::cout << "Flag -l has value: " << parser.getFlagValue("-l") << std::endl;
    }

    // Get value for -lang flag
    if (parser.hasFlag("-lang")) {
        std::cout << "Flag -lang has value: " << parser.getFlagValue("-lang") << std::endl;
    }

    // Get positional arguments
    std::vector<std::string> positionalArgs = parser.getPositionalArgs();
    std::cout << "Positional arguments: ";
    for (const auto& arg : positionalArgs) {
        std::cout << arg << " ";
    }
    std::cout << std::endl;

    // Check if other flags exist
    if (parser.hasFlag("-c")) {
        std::cout << "Flag -c has value: " << parser.getFlagValue("-c") << std::endl;
    }

    if (parser.hasFlag("-pack")) {
        std::cout << "Flag -pack has value: " << parser.getFlagValue("-pack") << std::endl;
    }

    return 0;
}
*/