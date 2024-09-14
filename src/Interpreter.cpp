#include <Values.h>
#include <AST.h>
#include <Generators.h>
#include <Utilities.h>
#include <Interpreter.h>
#include <ENV.h>

std::shared_ptr<Ecliptix::Values::RuntimeValue> evalProgram(Ecliptix::AST::Program* program, Ecliptix::Environment env) {
    if (!program) {
        std::cerr << "no program found??" << std::endl;
        exit(1);
	}
    std::shared_ptr<Ecliptix::Values::RuntimeValue> val;

    for (auto& stment : program->body) {
    	val = Ecliptix::Interpreter::evaluate(std::move(stment), env);
	}



    return val;
}

std::shared_ptr<Ecliptix::Values::RuntimeValue> evalIdentifier(Ecliptix::AST::IdentifierLiteral* ident, Ecliptix::Environment env) {
    if (!ident) {
        std::cerr << "no ident found" << std::endl;
        exit(1);
	}

	return env.lookupVar(ident->symbol);
}

std::shared_ptr<Ecliptix::Values::RuntimeValue> evalBinExprs(Ecliptix::AST::BinaryExpression* binop, Ecliptix::Environment env) {
	std::shared_ptr<Ecliptix::Values::RuntimeValue> lhsPtr = Ecliptix::Interpreter::evaluate(
    	std::shared_ptr<Ecliptix::AST::Statement>(std::move(binop->left)), env);
	Ecliptix::Values::RuntimeValue *lhs = lhsPtr.get();

	std::shared_ptr<Ecliptix::Values::RuntimeValue> rhsPtr = Ecliptix::Interpreter::evaluate(
    	std::shared_ptr<Ecliptix::AST::Statement>(std::move(binop->right)), env);
	Ecliptix::Values::RuntimeValue *rhs = rhsPtr.get();




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

		return Ecliptix::Generators::createNumberValue(result);
	} else {
		return Ecliptix::Generators::createNullValue();
	}
}

namespace Ecliptix::Interpreter {
	std::shared_ptr<Ecliptix::Values::RuntimeValue> evaluate(std::shared_ptr<Ecliptix::AST::Statement> astNode, Ecliptix::Environment env){
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
				return evalBinExprs(num, env);
			} break;
			case Ecliptix::AST::NodeType::Program:{
			    Ecliptix::AST::Program* num = dynamic_cast<Ecliptix::AST::Program*>(&*astNode);

		    	return evalProgram(num, env);
			} break;
			case Ecliptix::AST::NodeType::Identifier:{
				Ecliptix::AST::IdentifierLiteral* num = dynamic_cast<Ecliptix::AST::IdentifierLiteral*>(&*astNode);

		    	return evalIdentifier(num, env);
			}

			default:{
				std::cout << "This AST node is not yet set up" << std::endl;
				exit(1);
			} break;
				
		}
	}
}