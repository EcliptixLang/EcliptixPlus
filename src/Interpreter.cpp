#include <Values.h>
#include <AST.h>
#include <Generators.h>
#include <Utilities.h>
#include <Interpreter.h>

std::unique_ptr<Ecliptix::Values::RuntimeValue> evalProgram(Ecliptix::AST::Program* program) {
	std::unique_ptr<Ecliptix::Values::RuntimeValue> val = Ecliptix::Generators::createNullValue();

	for(const std::unique_ptr<Ecliptix::AST::Statement>& stment : program->body){
		val = Ecliptix::Interpreter::evaluate(*stment);
	}
	return val;
}
std::unique_ptr<Ecliptix::Values::RuntimeValue> evalBinExprs(Ecliptix::AST::BinaryExpression* binop) {
	Ecliptix::Values::RuntimeValue *lhs = Ecliptix::Interpreter::evaluate(*binop->left).get();
	Ecliptix::Values::RuntimeValue *rhs = Ecliptix::Interpreter::evaluate(*binop->right).get();

	if(lhs->type == Ecliptix::Values::ValueType::Number && rhs->type == Ecliptix::Values::ValueType::Number){
		return evalNumBinExprs(dynamic_cast<Ecliptix::Values::NumberValue&>(*lhs), dynamic_cast<Ecliptix::Values::NumberValue&>(*rhs), binop->_operator);
	}
}
std::unique_ptr<Ecliptix::Values::NumberValue> evalNumBinExprs(Ecliptix::Values::NumberValue lhs, Ecliptix::Values::NumberValue rhs, std::string oper){
	int result;
	if(oper == "+")
		result = lhs.value + rhs.value;
	else if(oper == "-")
		result = lhs.value - rhs.value;
	else if(oper == "/")
		result = lhs.value / rhs.value;
	else if(oper == "*")
		result = lhs.value * rhs.value;

	return std::make_unique<Ecliptix::Values::NumberValue>(Ecliptix::Values::ValueType::Number, result);
}


namespace Ecliptix::Interpreter {
	std::unique_ptr<Ecliptix::Values::RuntimeValue> evaluate(Ecliptix::AST::Statement astNode){
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
				Ecliptix::AST::BinaryExpression* num = dynamic_cast<Ecliptix::AST::BinaryExpression*>(&astNode);
				return evalBinExprs(num);
			}
			case Ecliptix::AST::NodeType::Program:{
				Ecliptix::AST::Program* num = dynamic_cast<Ecliptix::AST::Program*>(&astNode);
				return evalProgram(num);
			}

			default:{
				std::cout << "This AST node is not yet set up" << std::endl;
				exit(1);
			}
				
		}
	}
}