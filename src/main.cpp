#include <Parser.h>
#include <Interpreter.h>
#include <iostream>

int main() {
    Ecliptix::Parser::Parser parser;

    std::cout << "> ";
	std::string code;
	std::getline(std::cin, code);

    Ecliptix::AST::Program program = parser.produceAST(code);
	std::unique_ptr<Ecliptix::AST::Program> pointer = std::make_unique<Ecliptix::AST::Program>(program.kind, std::move(program.body));

	Ecliptix::Values::RuntimeValue* val = Ecliptix::Interpreter::evaluate(std::move(pointer)).get();
	
	switch(val->type){
		case Ecliptix::Values::ValueType::Null:
			std::cout << "NullValue" << std::endl;
		break;
		case Ecliptix::Values::ValueType::Number:
			std::cout << "Number, Value: " << dynamic_cast<Ecliptix::Values::NumberValue&>(*val).value << std::endl;
		break;
		default:
			std::cout << "IDK" << std::endl;
	}
	
    return 0;
}