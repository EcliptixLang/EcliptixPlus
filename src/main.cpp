#include <Parser.hpp>
#include <Interpreter.hpp>
#include <Lexer.hpp>
#include <iostream>

int main() {
    Parser parser;

    std::cout << "> ";
	std::string code;
	std::getline(std::cin, code);
	if(code == "exit")
		exit(0);

	Environment env;

	std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);

    AST::Program* program = dynamic_cast<AST::Program*>(ast.get());

	if(program){
		Interpreter::evaluate(std::move(ast), std::move(env));
	} else {
		return 2;
	}
    return 0;
}