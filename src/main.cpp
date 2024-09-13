#include <Parser.h>
#include <Interpreter.h>
#include <iostream>

int main() {
    Ecliptix::Parser::Parser parser;

    std::cout << "> ";
	std::string code;
	std::getline(std::cin, code);
	std::cout << code;

    Ecliptix::AST::Program program = parser.produceAST(code);

	Ecliptix::Values::RuntimeValue* val = Ecliptix::Interpreter::evaluate(program).get();
	if(val->type == Ecliptix::Values::ValueType::Number){
		std::cout << "Number, Value: " << dynamic_cast<Ecliptix::Values::NumberValue&>(*val).value << std::endl;
	} else if(val->type == Ecliptix::Values::ValueType::Null){
		std::cout << "NullValue" << std::endl;
	}
    return 0;
}