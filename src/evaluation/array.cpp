#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IArray(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Array* arr = dynamic_cast<AST::Array*>(astNode.get());
	std::vector<std::shared_ptr<Values::Runtime>> array{};

	for(auto& val : arr->elements){
		std::shared_ptr<Values::Runtime> v = this->evaluate(val, env);
		array.push_back(v);
	}

	return std::make_shared<Values::Array>(array);
}
