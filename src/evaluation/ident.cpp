#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IIdent(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Identifier * ident = dynamic_cast<AST::Identifier*>(astNode.get());
	std::shared_ptr<Values::Runtime> val = env.getVariable(ident->name).value;
    env.setVariableSafe(ident->name, val);
    
	return val;
}