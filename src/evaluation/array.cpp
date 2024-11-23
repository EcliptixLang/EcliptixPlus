#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IArray(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Array* arr = dynamic_cast<AST::Array*>(astNode.get());
	std::vector<std::unique_ptr<Values::Runtime>> array{};

	for(auto& val : arr->elements){
		std::unique_ptr<Values::Runtime> v = this->evaluate(val, env);
		array.push_back(std::move(v));
	}

	return std::make_unique<Values::Array>(Values::Array(std::move(array)));
}
