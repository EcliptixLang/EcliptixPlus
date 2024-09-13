#include <Values.h>
#include <AST.h>
#include <Generators.h>
#include <Utilities.h>
#include <Interpreter.h>

std::unique_ptr<Ecliptix::Values::RuntimeValue> evalProgram(Ecliptix::AST::Program* program) {
    if (!program) {
        std::cerr << "no program found??" << std::endl;
        exit(1);
	}
    std::unique_ptr<Ecliptix::Values::RuntimeValue> val;

    for (const std::unique_ptr<Ecliptix::AST::Statement>& stment : program->body) {
        val = Ecliptix::Interpreter::evaluate(*stment);
    }

    return val;
}

std::unique_ptr<Ecliptix::Values::RuntimeValue> evalBinExprs(Ecliptix::AST::BinaryExpression* binop) {
	Ecliptix::Values::RuntimeValue *lhs = Ecliptix::Interpreter::evaluate(*binop->left).get();
	Ecliptix::Values::RuntimeValue *rhs = Ecliptix::Interpreter::evaluate(*binop->right).get();

	if(lhs->type == Ecliptix::Values::ValueType::Number && rhs->type == Ecliptix::Values::ValueType::Number){
		Ecliptix::Values::NumberValue lhss = dynamic_cast<Ecliptix::Values::NumberValue&>(*lhs);
		Ecliptix::Values::NumberValue rhss = dynamic_cast<Ecliptix::Values::NumberValue&>(*rhs);
		std::string oper = binop->_operator;
		int result;
		if(oper == "+")
			result = lhss.value + rhss.value;
		else if(oper == "-")
			result = lhss.value - rhss.value;
		else if(oper == "/")
			result = lhss.value / rhss.value;
		else if(oper == "*")
			result = lhss.value * rhss.value;

		return std::make_unique<Ecliptix::Values::NumberValue>(result);
	} else {
		return Ecliptix::Generators::createNullValue();
	}
}

namespace Ecliptix::Interpreter {
	std::unique_ptr<Ecliptix::Values::RuntimeValue> evaluate(Ecliptix::AST::Statement astNode){
		std::cout << "INTERPRETER START" << std::endl;
		switch(astNode.kind){
			case Ecliptix::AST::NodeType::NumericLiteral:{
				const Ecliptix::AST::NumericLiteral* num = dynamic_cast<const Ecliptix::AST::NumericLiteral*>(&astNode);
				return Ecliptix::Generators::createNumberValue(num->value);
			}
			case Ecliptix::AST::NodeType::NullLiteral:{
				const Ecliptix::AST::NullLiteral* num = dynamic_cast<const Ecliptix::AST::NullLiteral*>(&astNode);
				return Ecliptix::Generators::createNullValue();
			}
			case Ecliptix::AST::NodeType::BinaryExpression:{
				std::cout << "BINEXP?" << std::endl;
				Ecliptix::AST::BinaryExpression* num = dynamic_cast<Ecliptix::AST::BinaryExpression*>(&astNode);
				return evalBinExprs(num);
			}
			case Ecliptix::AST::NodeType::Program:{
				std::cout << "PROGRAM FOUND" << std::endl;
				Ecliptix::AST::Program* num = dynamic_cast<Ecliptix::AST::Program*>(&astNode);
				std::cout << Ecliptix::AST::StringifyNodeTypes(num->body[0].get()->kind) << std::endl; // segfault here
				std::cout << "ECECUEZRGOQERG" << std::endl;
				return evalProgram(num);
			}

			default:{
				std::cout << "This AST node is not yet set up" << std::endl;
				exit(1);
			}
				
		}
	}
}