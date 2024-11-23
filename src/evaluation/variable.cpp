#include <Interpreter.hpp>
#include <memory>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IVariable(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::VariableExpr* var = dynamic_cast<AST::VariableExpr*>(astNode.get());
	std::unique_ptr<Values::Runtime> val = this->evaluate(var->Value, env);

	if(val->type() == var->Type || var->Type == "auto"){
		env.setVariable(var->Name, std::move(val), val->type(), var->constant);
		return val;
	} else if(val->type() == "return"){
		Values::ReturnedValue* ret = dynamic_cast<Values::ReturnedValue*>(std::move(val->clone().get()));
		env.setVariable(var->Name, std::move(ret->value->clone()), val->type(), var->constant);
		return std::move(ret->value);
	} else {
		std::cout << "Error while declaring variable '" << var->Name << "', because it seems like it's requiring type '" << var->Type << "' and the value applied to it was of type '" << val->type() << "'.\n";
		exit(6);
	}
}