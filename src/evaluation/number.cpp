#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::INumber(std::shared_ptr<AST::ExprAST>& astNode){
	AST::NumberExpr* number = dynamic_cast<AST::NumberExpr*>(astNode.get());
	return std::make_shared<Values::Number>(Values::Number(number->Value));
}