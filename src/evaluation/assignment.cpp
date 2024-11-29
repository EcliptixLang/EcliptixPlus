#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IAssignment(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::AssignmentExpr* assignment = dynamic_cast<AST::AssignmentExpr*>(astNode.get());
	if(assignment->assignee->getType() != AST::Nodes::Identifier){
		std::cout << "Invalid assignment.\n";
	}
	AST::Identifier* var = dynamic_cast<AST::Identifier*>(assignment->assignee.get());
	
	Variable vall = env.getVariable(var->name);
	if(vall.value != nullptr){
		
		std::shared_ptr<Values::Runtime> val = this->evaluate(assignment->value, env); 
		if(vall.type == val->type()){
			env.setVariableSafe(var->name, val, vall.constant);
			return std::make_shared<Values::Null>(Values::Null());
		} else {
			std::cout << "Error while declaring variable '" << var->name << "', because it seems like it's requiring type '" << vall.type << "' and the value applied to it was of type '" << val->type() << "'.\n";
			exit(6);
		}
	} else {
		return std::make_shared<Values::Null>(Values::Null());
	}
}