#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IIdent(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	std::shared_ptr<Values::Runtime> val = env.getVariable(dynamic_cast<AST::Identifier*>(astNode.get())->name).value;
    env.setVariableSafe(dynamic_cast<AST::Identifier*>(astNode.get())->name, val->clone());
    
	return val;
}