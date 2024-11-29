#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

bool trotli(std::shared_ptr<Values::Runtime>& conditional){
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

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

std::shared_ptr<Values::Runtime> Interpreter::IWhen(std::shared_ptr<AST::ExprAST>& astNode, Environment& env){
    bool truu = true;
    AST::WhenDeclaration* whilee = dynamic_cast<AST::WhenDeclaration*>(astNode.get());

    Environment enva; enva.setParent(&env); enva.setup();
    std::shared_ptr<Values::Runtime> cond = this->evaluate(whilee->conditional, env);
        std::shared_ptr<AST::ExprAST> clone = whilee->clone();
        AST::WhenDeclaration* wil = dynamic_cast<AST::WhenDeclaration*>(clone.get());
        std::vector<std::shared_ptr<AST::ExprAST>> thingy = wil->consequent;
        if(trotli(cond)){
            for(auto& thing : thingy){
                std::shared_ptr<Values::Runtime> val = this->evaluate(thing, enva);
                if(val != nullptr){
                    if (val->type() == "break"){
                        break;
                    }
                }
            }
        } else {
            env.events[gen_random(5)] = { whilee->conditional, whilee->consequent };
        }
    return std::make_shared<Values::Null>(Values::Null());
}