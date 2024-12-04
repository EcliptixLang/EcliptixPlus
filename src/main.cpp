/** 
 * @name Ecliptix
 * @author SkyOPG
 * @brief A Programming language meant to be easy.
 * @details A cool general purpose programming language for use :D
 * @version 0.2
 */

#include <Parser.hpp>
#include <Interpreter.hpp>
//#include <config.hpp>
#include <winutils.hpp>
#include <iostream>
//#include <curl/curl.h>
#include <vector>
#include <string>
#include <unordered_map>

std::string file = "[project]\nname = \"Project\"\nversion = \"1.0\"\nlibs = []\n[interpreter]\nmax_when = 10\nuse_new_syntax = false\n[types]\nstring = \"String\"\nnumber = \"Numbah\"\nobject = \"Object\"\narray = \"Array\"\nboolean = \"Boolean\"\nauto = \"Auto\"\nnull = \"Null\"\nshell = \"Shell\"\n[keywords]\nlock = \"lock\"\nfun = \"fun\"\nif = \"if\"\nelse = \"else\"\nfor = \"for\"\nwhile = \"while\"\nreturn = \"return\"\nset = \"set\"\nbreak = \"break\"\ntake = \"take\"\nfrom = \"from\"\ngive = \"give\"\nusing = \"using\"\nwhen = \"when\"\nskip = \"skip\"";

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
        std::cout << "\t-r, --repl: Run a REPL\n";
        std::cout << "\t-h, --help: Show help commands\n";
        std::cout << "\t-i, --init: Initialize a new Ecliptix project\n";
        exit(0);
    }

    if(argParser.hasFlag("-i") || argParser.hasFlag("--init")){
        Utilities::writeFile("config.ecc", file);
        std::cout << "Created config.ecc with default settings!\n";
        exit(0);
    }

    Settings settings;
    ParentENV.setup();
    if(argParser.hasFlag("-r") || argParser.hasFlag("--repl"))
        return runREPL(settings);
    std::vector<std::string> files = argParser.getPositionalArgs();
	for(auto file : files){
        runFile(file, settings);
    }
}