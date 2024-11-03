#include <Parser.hpp>
#include <Interpreter.hpp>
#include <Lexer.hpp>
#include <iostream>
#include <winutils.hpp>
#include <Utilities.hpp>

Parser parser;
Environment ParentENV;

int runFile(std::string filePath){
	std::string code = Utilities::readFile(filePath);

	std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);

	Interpreter::evaluate(ast, ParentENV);
	
	return 0;
}

int runREPL(){
	while(true){
		std::cout << "> ";
		std::string code;
		std::getline(std::cin, code);
		if(code == "exit")
			exit(0);

		std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
	    AST::Program* program = dynamic_cast<AST::Program*>(ast.get());

		if(program){
			Interpreter::evaluate(ast, ParentENV);
		} else {
			std::cout << "can't evaluate that!\n";
		}
	}

	return 0;
}

int main(int argc, char* argv[]) {
	if(argc > 1)
		return runFile(argv[1]);
	else
		return runREPL();
}