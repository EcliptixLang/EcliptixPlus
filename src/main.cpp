#include <Parser.h>
#include <Interpreter.h>
#include <iostream>

int main() {
	while(true){
    Ecliptix::Parser::Parser parser;

    std::cout << "> ";
	std::string code;
	std::getline(std::cin, code);
	if(code == "exit")
		exit(0);
	Ecliptix::Environment env;

    Ecliptix::AST::Program program = parser.produceAST(code);
	std::shared_ptr<Ecliptix::AST::Program> pointer = std::make_shared<Ecliptix::AST::Program>(program.kind, std::move(program.body));

	std::shared_ptr<Ecliptix::Values::RuntimeValue> rval = Ecliptix::Interpreter::evaluate(std::move(pointer), env);
	Ecliptix::Values::RuntimeValue * val = rval.get();

	
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
	}
    return 0;
}