#include <Values.hpp>
#include <AST.hpp>
#include <Utilities.hpp>
#include <Interpreter.hpp>
#include <ENV.hpp>
#include <FunctionValues.hpp>

namespace Interpreter {
	std::unique_ptr<Values::Runtime> evaluate(std::unique_ptr<AST::ExprAST> astNode, Environment env){

		std::string type = astNode.get()->getType();
		if(type == "Number"){
			AST::NumberExpr* number = dynamic_cast<AST::NumberExpr*>(std::move(astNode.get()));
			std::cout << "numbah " << number->Value << "\n";
			return std::make_unique<Values::Number>(Values::Number(number->Value));
		}else if(type == "Program"){
			AST::Program* program = dynamic_cast<AST::Program*>(std::move(astNode.get()));
			int i = 0;
			while(i != program->body.size()){
				std::unique_ptr<AST::ExprAST> expr = std::move(program->body[i]);
				evaluate(std::move(expr), std::move(env));
				i++;
			}
			std::cout << "program done!\n";
		} else if(type == "VarExpr") {
			AST::VariableExpr* var = dynamic_cast<AST::VariableExpr*>(std::move(astNode.get()));

			env.declareVar(var->Name, evaluate(std::move(var->Value), std::move(env)), var->constant);
		} else if(type == "CallExpr") {
			AST::CallExpr* call = dynamic_cast<AST::CallExpr*>(std::move(astNode.get()));
			std::vector<std::unique_ptr<Values::Runtime>> args{};
			for(auto& arg : call->Args){
				args.push_back(evaluate(std::move(arg), std::move(env)));
			}
			std::string calleeType = call->Callee.get()->getType();
			if(calleeType != "Identifier")
				throw std::runtime_error("Callee is not an identifier");
			AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
			std::unique_ptr<Values::Runtime> fun = env.lookupVar(ident->name);
			if(fun.get()->type() != "native-fn")
				throw std::runtime_error("Non function is trying to get called.");
			NativeFN* fn = dynamic_cast<NativeFN*>(std::move(fun.get()));
			
			fn->call(std::move(args), env);

		} else {
			std::cerr << "This AST Node is not yet set up for interpretation.\n" << "- Type: " << type << "\n";
			exit(1);
		}
	}
}