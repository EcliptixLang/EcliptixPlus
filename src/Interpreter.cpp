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

    for (auto& stment : program->body) {
    	val = Ecliptix::Interpreter::evaluate(std::move(stment));
	}



    return val;
}

std::unique_ptr<Ecliptix::Values::RuntimeValue> evalBinExprs(Ecliptix::AST::BinaryExpression* binop) {
	Ecliptix::Values::RuntimeValue *lhs = Ecliptix::Interpreter::evaluate(
    	std::unique_ptr<Ecliptix::AST::Statement>(std::move(binop->left))).get();

	Ecliptix::Values::RuntimeValue *rhs = Ecliptix::Interpreter::evaluate(
    	std::unique_ptr<Ecliptix::AST::Statement>(std::move(binop->right))).get();



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

		std::cout << "returning numval with " << result << std::endl;

		return Ecliptix::Generators::createNumberValue(result);
	} else {

		std::cout << "returning null" << std::endl;
		return Ecliptix::Generators::createNullValue();
	}
}

namespace Ecliptix::Interpreter {
	std::unique_ptr<Ecliptix::Values::RuntimeValue> evaluate(std::unique_ptr<Ecliptix::AST::Statement> astNode){
		switch(astNode->kind){
			case Ecliptix::AST::NodeType::NumericLiteral:{
				const Ecliptix::AST::NumericLiteral* num = dynamic_cast<const Ecliptix::AST::NumericLiteral*>(astNode.get());
				return Ecliptix::Generators::createNumberValue(num->value);
			} break;
			case Ecliptix::AST::NodeType::NullLiteral:{
				const Ecliptix::AST::NullLiteral* num = dynamic_cast<const Ecliptix::AST::NullLiteral*>(astNode.get());
				return Ecliptix::Generators::createNullValue();
			} break;
			case Ecliptix::AST::NodeType::BinaryExpression:{
				Ecliptix::AST::BinaryExpression* num = dynamic_cast<Ecliptix::AST::BinaryExpression*>(astNode.get());
				return evalBinExprs(num);
			} break;
			case Ecliptix::AST::NodeType::Program:{
			    Ecliptix::AST::Program* num = dynamic_cast<Ecliptix::AST::Program*>(&*astNode);

		    	return evalProgram(num);
			} break;

			default:{
				std::cout << "This AST node is not yet set up" << std::endl;
				exit(1);
			} break;
				
		}
	}
}