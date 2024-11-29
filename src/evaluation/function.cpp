#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IFunction(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Function* fun = dynamic_cast<AST::Function*>(astNode.get());

	env.setVariable(fun->name, std::make_shared<Values::Function>(Values::Function(fun->body, fun->params, fun->name, fun->type)), "function", true);

	return env.getVariable(fun->name).value;
}