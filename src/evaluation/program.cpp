#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IProgram(std::unique_ptr<AST::ExprAST>& astNode, Environment& env) {
    string a = AST::stringifyAST(astNode->getType());
	AST::Program* program = dynamic_cast<AST::Program*>(astNode.get());
    if (!program) {
        throw std::runtime_error("Invalid AST Node: Expected AST::Program.");
    }
    
    int i = 0;
    std::unique_ptr<Values::Runtime> val;

    while (i < program->body.size()) {
        std::unique_ptr<AST::ExprAST> expr = std::move(program->body[i]);
        val = this->evaluate(expr, env);
        i++;
    }

    return val;
}