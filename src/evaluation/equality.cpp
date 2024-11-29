#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

bool isTruthy(std::shared_ptr<Values::Runtime>& left, Lexer::Token op, std::shared_ptr<Values::Runtime>& right){
	std::string oap = op.value;
	if(oap == "=="){
		return left->stringValue() == right->stringValue();
	} else if(oap == "!="){
		return left->stringValue() != right->stringValue();	
	} else if(oap == ">="){
		if(left->type() == "number" && right->type() == "number"){
			int l = dynamic_cast<Values::Number*>(left.get())->value;
			int r = dynamic_cast<Values::Number*>(right.get())->value;
			return l >= r;
		} else if (left->type() == "null" || right->type() == "null"){
			return false;
		} else {
			return false;
		}
	} else if(oap == "<="){
		if(left->type() == "number" && right->type() == "number"){
			int l = dynamic_cast<Values::Number*>(left.get())->value;
			int r = dynamic_cast<Values::Number*>(right.get())->value;
			return l <= r;
		} else if (left->type() == "null" || right->type() == "null"){
			return false;
		} else {
			return false;
		}
	} else if(oap == "@="){
		if(left->type() == "array"){
			std::vector<std::shared_ptr<Values::Runtime>> l = dynamic_cast<Values::Array*>(left.get())->elements;
			for(auto& lol : l){
				if(lol->stringValue() == right->stringValue())
					return true;
			}
			return false;
		} else {
			return false;
		}
	} else if(oap == "<"){
		if(left->type() == "number" && right->type() == "number"){
			int l = dynamic_cast<Values::Number*>(left.get())->value;
			int r = dynamic_cast<Values::Number*>(right.get())->value;
			return l < r;
		} else if (left->type() == "null" || right->type() == "null"){
			return false;
		} else {
			return false;
		}
	} else if(oap == ">") {
		if(left->type() == "number" && right->type() == "number"){
			int l = dynamic_cast<Values::Number*>(left.get())->value;
			int r = dynamic_cast<Values::Number*>(right.get())->value;
			return l > r;
		} else if (left->type() == "null" || right->type() == "null"){
			return false;
		} else {
			return false;
		}
	}
	return false;
}

std::shared_ptr<Values::Runtime> Interpreter::IEqu(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::EquExpr* equ = dynamic_cast<AST::EquExpr*>(astNode.get());
	std::shared_ptr<Values::Runtime> left = this->evaluate(equ->left, env);
	std::shared_ptr<Values::Runtime> right = this->evaluate(equ->right, env);

	if(isTruthy(left, equ->oper, right)){
		return std::make_shared<Values::Boolean>(true);
	}
	else return std::make_shared<Values::Boolean>(false);
}