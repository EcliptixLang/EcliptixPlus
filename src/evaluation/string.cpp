#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

void replaceAll(string &str, const string &from, const string &to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
}

std::unique_ptr<Values::Runtime> Interpreter::IString(std::unique_ptr<AST::ExprAST>& astNode){
	AST::StringExpr* str = dynamic_cast<AST::StringExpr*>(astNode.get());
	string val = str->Value;
	replaceAll(val, "\\n", "\n");
	replaceAll(val, "\\t", "\t");
	replaceAll(val, "\\b", "\b");

	return std::make_unique<Values::String>(Values::String(val));
}