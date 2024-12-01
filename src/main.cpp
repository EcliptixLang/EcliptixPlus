/** 
 * @name Ecliptix
 * @author SkyOPG
 * @brief A Programming language meant to be easy.
 * @details A cool general purpose programming language for use :D
 * @version 0.2
 */

#include <Parser.hpp>
#include <Interpreter.hpp>
#include <config.hpp>
#include <winutils.hpp>
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
    std::unordered_map<std::string, std::vector<std::string>> flags;
    std::vector<std::string> positionalArgs;

    void parseArguments(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg[0] == '-') {
                if (i + 1 < argc && argv[i + 1][0] != '-') {
                    flags[arg].push_back(argv[i + 1]);
                    ++i;
                } else {
                    flags[arg].push_back("");
                }
            } else {
                positionalArgs.push_back(arg);
            }
        }
    }
};

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
    ArgumentParser argParser(argc, argv);
    if(argParser.hasFlag("-h") || argParser.hasFlag("--help")){
        std::cout << "Ecliptix Plus help menu\n";
        std::cout << "Usage: ecx [arguments] (file1, file2...)\n";
        std::cout << "\t-r, --repl: run a REPL\n";
        std::cout << "\t-h, --help: show help commands\n";
        exit(0);
    }
    Settings settings = config();
    ParentENV.setup();
    if(argParser.hasFlag("-r") || argParser.hasFlag("--repl"))
        return runREPL(settings);
    std::vector<std::string> files = argParser.getPositionalArgs();
	for(auto file : files){
        runFile(file, settings);
    }
}