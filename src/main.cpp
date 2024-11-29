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

Parser parser;
Environment ParentENV;
Interpreter irpr;

int runFile(std::string filePath){
	std::string code = Utilities::readFile(filePath);

	std::shared_ptr<AST::ExprAST> ast = parser.produceAST(code);
	irpr.evaluate(ast, ParentENV);
	
	return 0;
}

int runREPL(){
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
    ParentENV.setup();
	if(argc > 1)
		return runFile(argv[1]);
	else
		return runREPL();
}