/** 
 * @name Ecliptix
 * @author SkyOPG
 * @brief A Programming language meant to be easy.
 * @details A cool general purpose programming language for use :D
 * @version 0.2
 */

#include <Parser.hpp>
#include <Interpreter.hpp>
#include <winutils.hpp>
/*
#include <iostream>
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

    std::vector<std::string> getFlagValues(const std::string& flag) const {
        auto it = flags.find(flag);
        if (it != flags.end()) {
            return it->second;
        }
        return {};
    }

    std::vector<std::string> getPositionalArgs() const {
        return positionalArgs;
    }

private:
    std::unordered_map<std::string, std::vector<std::string>> flags;  // Stores flags and their values (multiple values allowed)
    std::vector<std::string> positionalArgs;  // Stores positional arguments

    void parseArguments(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg[0] == '-') {  // It's a flag
                if (i + 1 < argc && argv[i + 1][0] != '-') {
                    // Flag with value
                    flags[arg].push_back(argv[i + 1]);
                    ++i;  // Skip next argument as it's the value of the flag
                } else {
                    // Flag without value
                    flags[arg].push_back("");
                }
            } else {  // Positional argument (not a flag)
                positionalArgs.push_back(arg);
            }
        }
    }
};
*/
#include <config.hpp>

Environment ParentENV;
Interpreter irpr;

int runFile(std::string filePath, Settings settings){
	std::string code = Utilities::readFile(filePath);
    Parser parser(settings);
    irpr.settings = settings;
    ParentENV.settings = settings;
	std::shared_ptr<AST::ExprAST> ast = parser.produceAST(code);
	irpr.evaluate(ast, ParentENV);
	
	return 0;
}

int runREPL(Settings settings){
    Parser parser(settings);
    while(true){
        std::cout << "> ";
        std::string code;
        std::getline(std::cin, code);
        if(code == "exit")
            exit(0);

        std::shared_ptr<AST::ExprAST> ast = parser.produceAST(code);
        irpr.evaluate(ast, ParentENV);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    Settings settings = config();
    ParentENV.setup();
	if(argc > 1)
		return runFile(argv[1], settings);
	else
		return runREPL(settings);
}