#include <Values.hpp>
#include <AST.hpp>
#include <Utilities.hpp>
#include <Interpreter.hpp>
#include <ENV.hpp>
/*
std::unique_ptr<Values::Runtime> evalProgram(AST::Program* program, Environment env) {
    if (!program) {
        std::cerr << "no program found??" << std::endl;
        exit(1);
	}
    std::unique_ptr<Values::Runtime> val;

    for (auto& stment : program.body) {
    	val = Interpreter::evaluate(std::move(stment), env);
	}



    return val;
}

std::unique_ptr<Values::RuntimeValue> evalIdentifier(AST::IdentifierLiteral* ident, Environment env) {
    if (!ident) {
        std::cerr << "no ident found" << std::endl;
        exit(1);
	}

	return env.lookupVar(ident->symbol);
}

std::unique_ptr<Values::RuntimeValue> evalBinExprs(AST::BinaryExpression* binop, Environment env) {
	std::unique_ptr<Values::RuntimeValue> lhsPtr = Interpreter::evaluate(
    	std::unique_ptr<AST::Statement>(std::move(binop->left)), env);
	Values::RuntimeValue *lhs = lhsPtr.get();

	std::unique_ptr<Values::RuntimeValue> rhsPtr = Interpreter::evaluate(
    	std::unique_ptr<AST::Statement>(std::move(binop->right)), env);
	Values::RuntimeValue *rhs = rhsPtr.get();




	if(lhs->type == Values::ValueType::Number && rhs->type == Values::ValueType::Number){
		Values::NumberValue lhss = dynamic_cast<Values::NumberValue&>(*lhs);
		Values::NumberValue rhss = dynamic_cast<Values::NumberValue&>(*rhs);
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

		return Values::Number(result);
	} else {
		return Generators::createNullValue();
	}
}
*/
namespace Interpreter {
	std::unique_ptr<Values::Runtime> evaluate(std::unique_ptr<AST::ExprAST> astNode, Environment env){

		std::string type = astNode.get()->getType();
		if(type == "Number"){
			AST::NumberExpr* number = dynamic_cast<AST::NumberExpr*>(std::move(astNode.get()));
			std::cout << "numbah " << number->Value << "\n";
		}else if(type == "Program"){
			AST::Program* program = dynamic_cast<AST::Program*>(std::move(astNode.get()));
			for(auto& stmt : program->body){
				evaluate(std::move(stmt), std::move(env));
			}
		}
		/*switch(astNode->kind){
			case AST::NodeType::NumericLiteral:{ 
				const AST::NumericLiteral* num = dynamic_cast<const AST::NumericLiteral*>(astNode.get());
				return Generators::createNumberValue(num->value);
			} break;
			case AST::NodeType::NullLiteral:{
				const AST::NullLiteral* num = dynamic_cast<const AST::NullLiteral*>(astNode.get());
				return Generators::createNullValue();
			} break;
			case AST::NodeType::BinaryExpression:{
				AST::BinaryExpression* num = dynamic_cast<AST::BinaryExpression*>(astNode.get());
				return evalBinExprs(num, env);
			} break;
			case AST::NodeType::Program:{
			    AST::Program* num = dynamic_cast<AST::Program*>(&*astNode);

		    	return evalProgram(num, env);
			} break;
			case AST::NodeType::Identifier:{
				AST::IdentifierLiteral* num = dynamic_cast<AST::IdentifierLiteral*>(&*astNode);

		    	return evalIdentifier(num, env);
			}

			default:{
				std::cout << "This AST node is not yet set up" << std::endl;
				exit(1);
			} break;
				
		}*/
	}
}