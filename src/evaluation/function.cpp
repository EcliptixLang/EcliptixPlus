#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IFunction(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Function* fun = dynamic_cast<AST::Function*>(astNode.get());

	env.setVariable(fun->name, std::make_unique<Values::Function>(Values::Function(std::move(fun->body), fun->params, fun->name, fun->type)), "function", true);

	return env.getVariable(fun->name).value;
}