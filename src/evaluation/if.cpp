#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

bool truthy(std::unique_ptr<Values::Runtime>& conditional){
	if(conditional->type() == "boolean"){
		Values::Boolean* cond = dynamic_cast<Values::Boolean*>(conditional.get());
		const bool boolean = cond->value;
		if(boolean) return true; 
		else return false;
	}

	if(conditional){
		return true;
	} else {
		return false;
	}
}

std::unique_ptr<Values::Runtime> Interpreter::IIf(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	std::unique_ptr<Values::Runtime> val;
	AST::IfStatement* whilee = dynamic_cast<AST::IfStatement*>(astNode.get());
	Environment enva; enva.setParent(&env); enva.setup();
	std::unique_ptr<Values::Runtime> cond = this->evaluate(whilee->conditional, enva);
	if(truthy(cond)){
		for(auto& thing : whilee->consequent){
			val = this->evaluate(thing, enva);
			if(val != nullptr){
				if (val->type() == "skip"){
					break;	
				}
			}
		}
	} else {
		if(whilee->alternate.size() > 0){
			for(auto& thing : whilee->alternate){
				val = this->evaluate(thing, enva);
				if(val != nullptr){
					if (val->type() == "skip"){
						break;	
					}
				}
			}
		}
	}
	return std::make_unique<Values::Null>(Values::Null());
}