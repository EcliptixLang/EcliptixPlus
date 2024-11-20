#include <ENV.hpp>
#include <Parser.hpp>
#include <Values.hpp>

#define pointer std::unique_ptr

std::unique_ptr<Values::Runtime> thing(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg.get()->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value << "\n";

    return std::make_unique<Values::Null>(Values::Null());
}

namespace Interpreter {
	pointer<Values::Runtime> evaluate(pointer<AST::ExprAST>& astNode, Environment& env){
        AST::Nodes type = astNode.get()->getType();
//      std::cout << AST::stringifyAST(type) << std::endl;

        switch (type){
        case AST::Nodes::Number:
            return std::make_unique<Values::Number>(dynamic_cast<AST::NumberExpr*>(astNode.get())->Value);
        break;
        case AST::Nodes::String:
            return std::make_unique<Values::String>(dynamic_cast<AST::StringExpr*>(astNode.get())->Value);
        break;
        case AST::Nodes::Identifier: {
            std::unique_ptr<Values::Runtime> val = env.getVariable(dynamic_cast<AST::Identifier*>(astNode.get())->name).value;
            env.setVariableSafe(dynamic_cast<AST::Identifier*>(astNode.get())->name, val.get()->clone());
            return val;
        } break;

        case AST::Nodes::Program:{
            AST::Program* prog = dynamic_cast<AST::Program*>(astNode.get());

            for (int i = 0; i < prog->body.size(); i++)
            {
                evaluate(prog->body[i], env);
            }
            
        } break;

        case AST::Nodes::Binary: {
            AST::BinaryExpr* expr = dynamic_cast<AST::BinaryExpr*>(astNode.get());
            pointer<Values::Runtime> lhs = evaluate(expr->LHS, env);
            pointer<Values::Runtime> rhs = evaluate(expr->RHS, env);

            if(lhs.get()->type() == "null" || rhs.get()->type() == "null"){
                return std::make_unique<Values::Null>(Values::Null());
            } else if(lhs.get()->type() == "number" && rhs.get()->type() == "number"){
                Values::Number* lhsNumber = dynamic_cast<Values::Number*>(lhs.get());
                Values::Number* rhsNumber = dynamic_cast<Values::Number*>(rhs.get());
                int value;

                if(expr->Op == '+'){
                    value = lhsNumber->value + rhsNumber->value;
                } else if(expr->Op == '-'){
                    value = lhsNumber->value - rhsNumber->value; 
                } else if(expr->Op == '*'){
                    value = lhsNumber->value * rhsNumber->value; 
                } else if(expr->Op == '/'){
                    value = lhsNumber->value / rhsNumber->value; 
                } else {
                    throw std::runtime_error("Unknown instruction found");
                }

                return std::make_unique<Values::Number>(Values::Number(value));
            } else if(lhs.get()->type() == "string" || rhs.get()->type() == "string"){
                std::string lhsString = lhs.get()->stringValue();
                std::string rhsString = rhs.get()->stringValue();
                std::string value;

                if(expr->Op == '+'){
                    value = lhsString + rhsString;
                } else {
                    throw std::runtime_error("Unknown instruction found");
                }

                return std::make_unique<Values::String>(Values::String(value));
            } else {
                throw std::runtime_error("Bad news, only numbers & strings can be evaluated");
            }
        } break;

        case AST::Nodes::Call: {
            AST::CallExpr* call = dynamic_cast<AST::CallExpr*>(astNode.get());
            // uhh args
            std::vector<pointer<Values::Runtime>> args{};

            for(auto& arg : call->Args){
                args.push_back(evaluate(arg, env));
            }

            AST::Nodes calleeType = call->Callee.get()->getType();
            if(calleeType == AST::Nodes::Identifier){
                AST::Identifier* id = dynamic_cast<AST::Identifier*>(call->Callee.get());
                // custom placeholder function
                if(id->name == "out"){
                    return thing(std::move(args), env);
                }
            } else {
                throw std::runtime_error("Functions are not yet implemented");
            }
        } break;
        
        default:
            throw std::runtime_error("Unknown type");
        break;

        }
        return nullptr;
    }
}