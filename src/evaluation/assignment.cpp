#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IAssignment(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	std::cout << "??\n";
	AST::AssignmentExpr* assignment = dynamic_cast<AST::AssignmentExpr*>(astNode.get());
	std::cout << "??\n";
	if(assignment->assignee->getType() != AST::Nodes::Identifier){
	std::cout << "??\n";
		std::cout << "Invalid assignment.\n";
	}
	std::cout << "??\n";
	AST::Identifier* var = dynamic_cast<AST::Identifier*>(assignment->assignee.get());
	std::cout << "??\n";
	
	Variable vall = env.getVariable(var->name);
	std::cout << "??\n";
	if(vall.value != nullptr){
		std::cout << "??\n";
		
		std::unique_ptr<Values::Runtime> val = this->evaluate(assignment->value, env); 
		std::cout << "??\n";
		if(vall.type == val->type()){
			std::cout << "??\n";
			env.setVariableSafe(var->name, std::move(val), vall.constant);
			std::cout << "??\n";
			return std::make_unique<Values::Null>(Values::Null());
		} else {
			std::cout << "Error while declaring variable '" << var->name << "', because it seems like it's requiring type '" << vall.type << "' and the value applied to it was of type '" << val->type() << "'.\n";
			exit(6);
		}
	} else {
		return std::make_unique<Values::Null>(Values::Null());
	}
}