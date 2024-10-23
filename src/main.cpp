#include <Parser.hpp>
#include <Interpreter.hpp>
#include <Lexer.hpp>
#include <iostream>
#include <winutils.hpp>
#include <Utilities.hpp>

void runFile(std::string filePath){
	Parser parser;
	Environment ParentENV;
	std::string code = Utilities::readFile(filePath);

	std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
    AST::Program* program = dynamic_cast<AST::Program*>(ast.get());

	if(program){
		Interpreter::evaluate(std::move(ast), std::move(ParentENV));
	} else {
		return;
	}
}

void runREPL(){
	Parser parser;
	Environment ParentENV;

	while(true){
		std::cout << "> ";
		std::string code;
		std::getline(std::cin, code);
		if(code == "exit")
			exit(0);

		std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
	    AST::Program* program = dynamic_cast<AST::Program*>(ast.get());

		if(program){
			Interpreter::evaluate(std::move(ast), std::move(ParentENV));
		} else {
			std::cout << "can't evaluate that!\n";
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc > 1)
		runFile(argv[1]);
	else
		runREPL();

    return 0;
}