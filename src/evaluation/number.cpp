#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::INumber(std::unique_ptr<AST::ExprAST>& astNode){
	AST::NumberExpr* number = dynamic_cast<AST::NumberExpr*>(astNode.get());
	return std::make_unique<Values::Number>(Values::Number(number->Value));
}