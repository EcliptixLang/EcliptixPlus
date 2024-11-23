#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IObject(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Object* obj = dynamic_cast<AST::Object*>(astNode.get());
	std::map<string, std::unique_ptr<Values::Runtime>> props;

	for (const auto& val : obj->map) {
		AST::Element* element = dynamic_cast<AST::Element*>(std::move(val.get()));
		props[element->key] = (this->evaluate(element->value, env));
	}

	return std::make_unique<Values::Object>(Values::Object(std::move(props)));
}