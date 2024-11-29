#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> Interpreter::IProgram(std::shared_ptr<AST::ExprAST>& astNode, Environment& env) {
    string a = AST::stringifyAST(astNode->getType());
	AST::Program* program = dynamic_cast<AST::Program*>(astNode.get());
    if (!program) {
        throw std::runtime_error("Invalid AST Node: Expected AST::Program.");
    }
    
    int i = 0;
    std::shared_ptr<Values::Runtime> val;

    while (i < program->body.size()) {
        std::shared_ptr<AST::ExprAST> expr = program->body[i];
        val = this->evaluate(expr, env);
        i++;
    }

    return val;
}