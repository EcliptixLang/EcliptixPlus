#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IIdent(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	std::unique_ptr<Values::Runtime> val = env.getVariable(dynamic_cast<AST::Identifier*>(astNode.get())->name).value;
    env.setVariableSafe(dynamic_cast<AST::Identifier*>(astNode.get())->name, val->clone());
    
	return val;
}