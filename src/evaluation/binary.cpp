#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::IBinary(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::BinaryExpr* binexp = dynamic_cast<AST::BinaryExpr*>(astNode.get());

	std::unique_ptr<Values::Runtime> lhs = this->evaluate(binexp->LHS, env);
	std::unique_ptr<Values::Runtime> rhs = this->evaluate(binexp->RHS, env);

	string lhsType = lhs->type();
	string rhsType = rhs->type();

	if(lhs->type() == "number" && rhs->type() == "number"){
		int result;
		 Values::Number* numl = dynamic_cast<Values::Number*>(std::move(lhs.get()));
		Values::Number* numr = dynamic_cast<Values::Number*>(std::move(rhs.get()));
		if(binexp->Op == '+'){
			result = numl->value + numr->value;
		} else if(binexp->Op == '-'){
			result = numl->value - numr->value;
		} else if(binexp->Op == '*'){
			result = numl->value * numr->value;
		} else if(binexp->Op == '/'){
			result = numl->value / numr->value;
		} else {
			std::cout << "Unknown expression: " << numl->value << " " << binexp->Op << " " << numr->value << "\n"; 
		}

		return std::make_unique<Values::Number>(Values::Number(result));

	} else if(lhs->type() == "null" || lhs->type() == "null"){
			return std::make_unique<Values::Null>(Values::Null());
	} else {
		string lhsType = lhs->type();
		string rhsType = rhs->type();
		string value = "";
		if((lhsType != "function" || lhsType != "object") && (rhsType != "function" || rhsType != "object")){
			value.append(lhs->stringValue()).append(rhs->stringValue());
		} else {
			value = "null";
		}
		return std::make_unique<Values::String>(Values::String(value));
	}

	return std::make_unique<Values::Null>(Values::Null());
}