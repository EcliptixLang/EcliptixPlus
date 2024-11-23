#include <Interpreter.hpp>
#include <FunctionValues.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::ICall(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::CallExpr* call = dynamic_cast<AST::CallExpr*>(astNode.get());
	
	std::vector<std::unique_ptr<Values::Runtime>> args{};
	for(auto& arg : call->Args){
		args.push_back(Interpreter::evaluate(arg, env));
	}

	Nodes calleeType = call->Callee->getType();
	if(calleeType == Nodes::Member){
			std::unique_ptr<Values::Runtime> value;
			std::unique_ptr<Values::Runtime> val = IMember(call->Callee, env);
//			console.log(val->type());
			if(val->type() == "function"){
				Environment enva; enva.setParent(&env);
				Values::Function* fn = dynamic_cast<Values::Function*>(std::move(val->clone().get()));
				std::unique_ptr<Values::Runtime> value;

            	int i = 0;
            	for(auto& param : fn->parameters){
            		if(args.size() >= i)
            			env.setVariable(param, std::move(args[i]), args[i]->type(), false);
            		else
            			env.setVariable(param, std::make_unique<Values::Null>(Values::Null()), "null", false);
            		i++;
            	}

            	for(auto& expr : fn->body){
            		value = this->evaluate(expr, env);
            		if(value->type() == "return"){
            			Values::ReturnedValue* sum = dynamic_cast<Values::ReturnedValue*>(std::move(value->clone().get()));
            			if(sum->value->type() == fn->Type || fn->Type == "auto")
            				return std::move(sum->value);
            			else {
            				std::cout << "Error while running function '" << fn->name << "', it seems like it's requiring type '" << fn->Type << "' and the value it returned was of type '" << sum->value->type() << "'.\n";
            				exit(6);
            			}	
            		}
            	}

            	if(fn->Type == "void" || fn->Type == "nothing")
            		return value;
            	else {
            		std::cout << "Error (INTERPRETER):\n- Error Code: 7\n- Error Description: No return in non void function\n- Function name: '" << fn->name << "'\n- Required type: '" << fn->Type << "'\n";
            		exit(7);
            	}	

            	return value;
			} else if(val->type() == "native-fn"){
				NativeFN* sym = dynamic_cast<NativeFN*>(std::move(val.get()));
				return sym->call(std::move(args), env);
			}
//			else if(val->type() != "proto-fn")
			else
				throw std::runtime_error("Callee is not a function");
	}
	
	
	AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
	std::unique_ptr<Values::Runtime> fun = env.getVariable(ident->name).value;
	if(fun->type() == "native-fn"){
		NativeFN* fn = dynamic_cast<NativeFN*>(std::move(fun.get()));
		return fn->call(std::move(args), env);
	} else if(fun->type() == "function"){
		Environment enva; enva.setParent(&env);
		AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
    	Variable fun = env.getVariable(ident->name);

    	Values::Function* fn = dynamic_cast<Values::Function*>(std::move(fun.value->clone().get()));
    	std::unique_ptr<Values::Runtime> value;

    	int i = 0;
    	for(auto& param : fn->parameters){
    		if(args.size() >= i)
    			env.setVariable(param, std::move(args[i]), args[i]->type(), false);
    		i++;
    	}
    
    	for(auto& expr : fn->body){
    		value = this->evaluate(expr, env);
    		if(value->type() == "return"){
    			Values::ReturnedValue* sum = dynamic_cast<Values::ReturnedValue*>(std::move(value->clone().get()));
    			if(sum->value->type() == fn->Type || fn->Type == "auto")
    				return std::move(sum->value);
    			else {
    				std::cout << "Error while running function '" << fn->name << "', it seems like it's requiring type '" << fn->Type << "' and the value it returned was of type '" << sum->value->type() << "'.\n";
    				exit(6);
    			}	
    		}
    	}

    	if(fn->Type == "void" || fn->Type == "nothing")
    		return value;
    	else {
    		std::cout << "Error (INTERPRETER):\n- Error Code: 7\n- Error Description: No return in non void function\n- Function name: '" << fn->name << "'\n- Required type: '" << fn->Type << "'\n";
    		exit(7);
    	}	

    	return value;
	}

	throw std::runtime_error("Non function is trying to get called.");
}