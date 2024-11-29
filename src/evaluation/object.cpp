#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IObject(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Object* obj = dynamic_cast<AST::Object*>(astNode.get());
	std::map<string, std::shared_ptr<Values::Runtime>> props;

	for (const auto& val : obj->map) {
		AST::Element* element = dynamic_cast<AST::Element*>(val.get());
		props[element->key] = (this->evaluate(element->value, env));
	}

	return std::make_shared<Values::Object>(props);
}