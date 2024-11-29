#include <Interpreter.hpp>
#include <memory>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IVariable(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::VariableExpr* var = dynamic_cast<AST::VariableExpr*>(astNode.get());
	std::shared_ptr<Values::Runtime> val = this->evaluate(var->Value, env);

	if(val->type() == var->Type || var->Type == "auto"){
		env.setVariable(var->Name, val, val->type(), var->constant);
		return val;
	} else if(val->type() == "return"){
		Values::ReturnedValue* ret = dynamic_cast<Values::ReturnedValue*>(val.get());
		env.setVariable(var->Name, ret->value, val->type(), var->constant);
		return ret->value;
	} else {
		std::cout << "Error while declaring variable '" << var->Name << "', because it seems like it's requiring type '" << var->Type << "' and the value applied to it was of type '" << val->type() << "'.\n";
		exit(6);
	}
}