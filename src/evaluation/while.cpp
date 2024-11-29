#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

bool troti(std::shared_ptr<Values::Runtime>& conditional){
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

std::shared_ptr<Values::Runtime> Interpreter::IWhile(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
    bool truu = true;
    AST::WhileDeclaration* whilee = dynamic_cast<AST::WhileDeclaration*>(astNode.get());
    
    while(truu){
        Environment enva; enva.setParent(&env); enva.setup();
        std::shared_ptr<Values::Runtime> cond = this->evaluate(whilee->conditional, env);
        std::shared_ptr<AST::ExprAST> clone = whilee->clone();
        AST::WhileDeclaration* wil = dynamic_cast<AST::WhileDeclaration*>(clone.get());
        std::vector<std::shared_ptr<AST::ExprAST>> thingy = wil->consequent;
        if(troti(cond)){
            for(auto& thing : thingy){
                std::shared_ptr<Values::Runtime> val = this->evaluate(thing, enva);
                if(val != nullptr){
                    if (val->type() == "break"){
                        break;
                    }
                }
            }
        } else {
            truu = false;
        }
    }
    return std::make_shared<Values::Null>(Values::Null());
}