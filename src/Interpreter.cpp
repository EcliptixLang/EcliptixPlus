#include <Values.hpp>
#include <AST.hpp>
#include <Utilities.hpp>
#include <Interpreter.hpp>
#include <ENV.hpp>
#include <FunctionValues.hpp>

using Nodes = AST::Nodes; 

std::unique_ptr<Values::Runtime> IProgram(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> INumber(std::unique_ptr<AST::ExprAST>& astNode);
std::unique_ptr<Values::Runtime> ICall(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IVariable(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IString(std::unique_ptr<AST::ExprAST>& astNode);
std::unique_ptr<Values::Runtime> IIdent(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IBinary(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IObject(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IArray(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IMember(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
std::unique_ptr<Values::Runtime> IFunction(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
class Console {
	public:
	void log(std::string value){
		std::cout << value << std::endl;
	}
};
Console console;
namespace Interpreter {
	std::unique_ptr<Values::Runtime> evaluate(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){

		AST::Nodes type = astNode.get()->getType();
		switch (type){
			case Nodes::Number:
				return INumber(astNode);
			case Nodes::Program:
				return IProgram(astNode, env);
			case Nodes::Variable:
				return IVariable(astNode, env);
			case Nodes::Call:
				return ICall(astNode, env);
			case Nodes::String:
				return IString(astNode);
			case Nodes::Identifier:
				return IIdent(astNode, env);
			case Nodes::Binary:
				return IBinary(astNode, env);
			case Nodes::Array:
				return IArray(astNode, env);
			case Nodes::Member:
				return IMember(astNode, env);
			break;
			case Nodes::Function:
				return IFunction(astNode, env);
			case Nodes::Assignment:
				std::cout << "assignment what" << std::endl;
			break;
			case Nodes::Object:
				return IObject(astNode, env);
			break;
			case Nodes::Return: {
				AST::ReturnExpr* number = dynamic_cast<AST::ReturnExpr*>(std::move(astNode.get()));
				return std::make_unique<Values::ReturnedValue>(Values::ReturnedValue(Interpreter::evaluate(number->value, env)));
			} break;
			default:
				std::cerr 
					<< "This AST Node is not yet set up for interpretation.\n" 
					<< "- Type: " << AST::stringifyAST(type) << "\n";
				exit(1);
		}
	}
}

std::unique_ptr<Values::Runtime> INumber(std::unique_ptr<AST::ExprAST>& astNode){
	AST::NumberExpr* number = dynamic_cast<AST::NumberExpr*>(std::move(astNode.get()));
	return std::make_unique<Values::Number>(Values::Number(number->Value));
}
std::unique_ptr<Values::Runtime> IIdent(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Identifier* str = dynamic_cast<AST::Identifier*>(std::move(astNode.get()));

	std::unique_ptr<Values::Runtime> val = env.lookupVar(str->name);

	if(val != nullptr)
		return val;
	else
		throw std::runtime_error("cannot access null value");
}

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Move past the last replaced position
    }
}

std::unique_ptr<Values::Runtime> IString(std::unique_ptr<AST::ExprAST>& astNode){
	AST::StringExpr* str = dynamic_cast<AST::StringExpr*>(std::move(astNode.get()));
	std::string val = str->Value;
	replaceAll(val, "\\n", "\n");
	replaceAll(val, "\\t", "\t");
	replaceAll(val, "\\b", "\b");

	return std::make_unique<Values::String>(Values::String(val));
}

std::unique_ptr<Values::Runtime> IProgram(std::unique_ptr<AST::ExprAST>& astNode, Environment& env) {
    std::string a = AST::stringifyAST(astNode.get()->getType());
	AST::Program* program = dynamic_cast<AST::Program*>(astNode.get());
    if (!program) {
        throw std::runtime_error("Invalid AST Node: Expected AST::Program.");
    }
    
    int i = 0;
    std::unique_ptr<Values::Runtime> val;

    while (i < program->body.size()) {
        std::unique_ptr<AST::ExprAST> expr = std::move(program->body[i]);
        val = Interpreter::evaluate(expr, env);
        i++;
    }

    return val;
}


std::unique_ptr<Values::Runtime> IVariable(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::VariableExpr* var = dynamic_cast<AST::VariableExpr*>(std::move(astNode.get()));
	std::unique_ptr<Values::Runtime> val = Interpreter::evaluate(var->Value, env);

	if(val.get()->type() == var->Type || var->Type == "auto"){
		env.declareVar(var->Name, std::move(val), var->constant);
		return val;
	} else if(val->type() == "return"){
		Values::ReturnedValue* ret = dynamic_cast<Values::ReturnedValue*>(std::move(val.get()));
		env.declareVar(var->Name, std::move(ret->value), var->constant);
		return std::move(ret->value);
	} else {
		std::cout << "Error while declaring variable '" << var->Name << "', because it seems like it's requiring type '" << var->Type << "' and the value applied to it was of type '" << val.get()->type() << "'.\n";
		exit(6);
	}
}

std::unique_ptr<Values::Runtime> ICallFun(AST::CallExpr* call, Environment& env, std::vector<std::unique_ptr<Values::Runtime>>& args){
	AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
	std::unique_ptr<Values::Runtime> fun = env.lookupVar(ident->name);

	Values::Function* fn = dynamic_cast<Values::Function*>(std::move(fun.get()));
	std::unique_ptr<Values::Runtime> value;

	int i = 0;
	for(auto& param : fn->parameters){
		if(args.size() >= i)
			env.declareVar(param, std::move(args[i]), false);
		else
			env.declareVar(param, std::make_unique<Values::Null>(Values::Null()), false);
		i++;
	}
	
	for(auto& expr : fn->body){
		value = Interpreter::evaluate(expr, env);
		if(value.get()->type() == "return"){
			Values::ReturnedValue* sum = dynamic_cast<Values::ReturnedValue*>(std::move(value.get()));
			if(sum->value.get()->type() == fn->Type || fn->Type == "auto")
				return std::move(sum->value);
			else {
				std::cout << "Error while running function '" << fn->name << "', it seems like it's requiring type '" << fn->Type << "' and the value it returned was of type '" << sum->value.get()->type() << "'.\n";
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

std::unique_ptr<Values::Runtime> ICallMem(Values::Function* fn, Environment& env, std::vector<std::unique_ptr<Values::Runtime>>& args){
	std::unique_ptr<Values::Runtime> value;

	int i = 0;
	for(auto& param : fn->parameters){
		if(args.size() >= i)
			env.declareVar(param, std::move(args[i]), false);
		else
			env.declareVar(param, std::make_unique<Values::Null>(Values::Null()), false);
		i++;
	}
	
	for(auto& expr : fn->body){
		value = Interpreter::evaluate(expr, env);
		if(value.get()->type() == "return"){
			Values::ReturnedValue* sum = dynamic_cast<Values::ReturnedValue*>(std::move(value.get()));
			if(sum->value.get()->type() == fn->Type || fn->Type == "auto")
				return std::move(sum->value);
			else {
				std::cout << "Error while running function '" << fn->name << "', it seems like it's requiring type '" << fn->Type << "' and the value it returned was of type '" << sum->value.get()->type() << "'.\n";
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

std::unique_ptr<Values::Runtime> ICallMemNative(NativeFN* fn, Environment& env, std::vector<std::unique_ptr<Values::Runtime>>& args){
	std::unique_ptr<Values::Runtime> value = fn->call(std::move(args), env);

	return value;
}

std::unique_ptr<Values::Runtime> ICall(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::CallExpr* call = dynamic_cast<AST::CallExpr*>(std::move(astNode.get()));
	
	std::vector<std::unique_ptr<Values::Runtime>> args{};
	for(auto& arg : call->Args){
		args.push_back(Interpreter::evaluate(arg, env));
	}
	Environment enva; enva.setParent(env);

	Nodes calleeType = call->Callee.get()->getType();
	if(calleeType != Nodes::Identifier){
		if(calleeType == Nodes::Member){
			std::unique_ptr<Values::Runtime> value;
			AST::MemberExpr* callee = dynamic_cast<AST::MemberExpr*>(std::move(call->Callee.get()));
			std::unique_ptr<Values::Runtime> val = IMember(call->Callee, env);
			if(val.get()->type() == "function"){
				Values::Function* sym = dynamic_cast<Values::Function*>(std::move(val.get()));
				return ICallMem(sym, enva, args);
			} else if(val.get()->type() == "native-fn"){
				NativeFN* sym = dynamic_cast<NativeFN*>(std::move(val.get()));
				return ICallMemNative(sym, enva, args);
			}
//			else if(val.get()->type() != "proto-fn")
			else
				throw std::runtime_error("Value is not a function.");
		}
		throw std::runtime_error("Callee is not an identifier");
	}
	
	AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
	std::unique_ptr<Values::Runtime> fun = env.lookupVar(ident->name);
	if(fun.get()->type() == "native-fn"){
		NativeFN* fn = dynamic_cast<NativeFN*>(std::move(fun.get()));
		return fn->call(std::move(args), env);
	} else if(fun.get()->type() == "function"){
		return ICallFun(call, enva, args);
	}

	throw std::runtime_error("Non function is trying to get called.");
}

std::unique_ptr<Values::Runtime> IArray(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Array* arr = dynamic_cast<AST::Array*>(std::move(astNode.get()));
	std::vector<std::unique_ptr<Values::Runtime>> array{};

	for(auto& val : arr->elements){
		std::unique_ptr<Values::Runtime> v = Interpreter::evaluate(val, env);
		array.push_back(std::move(v));
	}

	return std::make_unique<Values::Array>(Values::Array(array));
}

std::unique_ptr<Values::Runtime> IObject(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Object* obj = dynamic_cast<AST::Object*>(std::move(astNode.get()));
	std::map<std::string, std::unique_ptr<Values::Runtime>> props;

	for (const auto& val : obj->map) {
		AST::Element* element = dynamic_cast<AST::Element*>(std::move(val.get()));
		props[element->key] = std::move(Interpreter::evaluate(element->value, env));
	}

	return std::make_unique<Values::Object>(Values::Object(props));
}

std::unique_ptr<Values::Runtime> IMember(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::MemberExpr* mem = dynamic_cast<AST::MemberExpr*>(std::move(astNode.get()));
	std::unique_ptr<Values::Runtime> val = Interpreter::evaluate(mem->object, env);
	AST::Identifier* id = dynamic_cast<AST::Identifier*>(std::move(mem->property.get()));
	std::string sym = id->name;
	if(val.get()->type() == "object"){
		Values::Object* obj = dynamic_cast<Values::Object*>(std::move(val.get()));
		std::unique_ptr<Values::Runtime> value = std::move(obj->props[sym]);

		if(value != nullptr){
			return value;
		} else {
			std::cout << "Property " << sym << " doesn't exist on the object.\n";
			exit(2);
		}
	} else {
		if(val.get()->type() == "string"){
			std::string value = dynamic_cast<Values::String*>(val.get())->value;
			
		}
	}
	return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> IAssignment(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::AssignmentExpr* assignment = dynamic_cast<AST::AssignmentExpr*>(std::move(astNode.get()));
	if(assignment->assignee.get()->getType() != AST::Nodes::Identifier){
		std::cout << "Invalid assignment.\n";
	}
	AST::Identifier* var = dynamic_cast<AST::Identifier*>(std::move(astNode.get()));
	std::string valueType = env.lookupVar(var->name).get()->type();
	std::unique_ptr<Values::Runtime> val = Interpreter::evaluate(assignment->value, env); 
	if(valueType == val.get()->type())
		return env.assignVar(var->name, std::move(val));
	else {
		std::cout << "Error while declaring variable '" << var->name << "', because it seems like it's requiring type '" << valueType << "' and the value applied to it was of type '" << val.get()->type() << "'.\n";
		exit(6);
	}
}

std::unique_ptr<Values::Runtime> IBinary(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::BinaryExpr* binexp = dynamic_cast<AST::BinaryExpr*>(std::move(astNode.get()));

	std::unique_ptr<Values::Runtime> lhs = Interpreter::evaluate(binexp->LHS, env);
	std::unique_ptr<Values::Runtime> rhs = Interpreter::evaluate(binexp->RHS, env);

	std::string lhsType = lhs.get()->type();
	std::string rhsType = rhs.get()->type();

	if(lhs.get()->type() == "number" && rhs.get()->type() == "number"){
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

	} else if(lhs.get()->type() == "null" || lhs.get()->type() == "null"){
			return std::make_unique<Values::String>(Values::String("null"));
	} else {
		std::string lhsType = lhs.get()->type();
		std::string rhsType = rhs.get()->type();
		std::string value = "";
		if((lhsType != "function" || lhsType != "object") && (rhsType != "function" || rhsType != "object")){
			value.append(lhs.get()->stringValue()).append(rhs.get()->stringValue());
		} else {
			value = "null";
		}
		return std::make_unique<Values::String>(Values::String(value));
	}

	return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> IIf(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){

}

std::unique_ptr<Values::Runtime> IFunction(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){
	AST::Function* fun = dynamic_cast<AST::Function*>(std::move(astNode.get()));

	return env.declareVar(fun->name, std::make_unique<Values::Function>(Values::Function(std::move(fun->body), fun->params, fun->name, fun->type)), true);
}

std::unique_ptr<Values::Runtime> IWhen(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){

}

std::unique_ptr<Values::Runtime> IWhile(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){

}

std::unique_ptr<Values::Runtime> IDSN(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){

}

std::string value(std::unique_ptr<AST::ExprAST>& astNode){

}