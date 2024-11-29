#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IMember(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	if(astNode == nullptr){
		std::cout << "astNode in IMember is a null std::shared_ptr jsyk\n";
	}
	AST::MemberExpr* mem = dynamic_cast<AST::MemberExpr*>(astNode.get());

	std::shared_ptr<Values::Runtime> val = this->evaluate(mem->object, env);
	if(val == nullptr){
		std::cout << "value coming from IMember is a null std::shared_ptr jsyk\n";
	}
	AST::Identifier* id = dynamic_cast<AST::Identifier*>(mem->property.get());
	string sym = id->name;

	if(val->type() == "object"){
		Values::Object* obj = dynamic_cast<Values::Object*>(val.get());
		std::shared_ptr<Values::Runtime> value = obj->props[sym];

		if(value != nullptr){
			return value;
		} else {
			std::cout << "Property " << sym << " doesn't exist on the object" << ".\n";
			exit(2);
		}
	} else {
		if(val->type() == "string"){
			string value = dynamic_cast<Values::String*>(val.get())->value;
			return val;
		}
	}
	throw std::runtime_error("Cannot access property on non-object value");
}