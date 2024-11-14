#include <Values.hpp>
#include <AST.hpp>
#include <Utilities.hpp>
#include <Interpreter.hpp>
#include <ENV.hpp>
#include <sstream>
#include <fstream>
#include <FunctionValues.hpp>
#include <array>

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

std::unique_ptr<Values::Runtime> ask(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg.get()->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value;
    std::string ans;
    std::getline(std::cin, ans);
    return std::make_unique<Values::String>(Values::String(ans));
}

std::unique_ptr<Values::Runtime> _throw(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::cout << "Error:\n- Code: 8\n- Description: ";
    thing(std::move(args), env);
    exit(8);

    return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> readFile(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    if(args[0].get() == nullptr){
        std::cout << "Cannot read null\n";
        exit(6);
    }

    std::string filecont = Utilities::readFile(args[0].get()->stringValue());

    return std::make_unique<Values::String>(Values::String(filecont));
}

std::unique_ptr<Values::Runtime> writeFile(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    if(args[0].get()->type() != "string"){
        std::cout << "Cannot read a non string\n";
        exit(6);
    }
    if(args[1].get()->type() != "string"){
        std::cout << "Cannot read a non string\n";
        exit(6);
    }
    
    Utilities::writeFile(args[0].get()->stringValue(), args[1].get()->stringValue());

    return std::make_unique<Values::Null>(Values::Null());
}

using Nodes = AST::Nodes; 
#define string std::string
#define pointer std::unique_ptr
#define makeptr std::make_unique

class Console {
	public:
	void log(string value){
		std::cout << value << std::endl;
	}
};

pointer<Values::Runtime> IProgram(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> INumber(pointer<AST::ExprAST>& astNode);
pointer<Values::Runtime> ICall(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IVariable(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IString(pointer<AST::ExprAST>& astNode);
pointer<Values::Runtime> IIdent(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IBinary(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IObject(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IArray(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IMember(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IFunction(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IAssignment(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IWhile(pointer<AST::ExprAST>& astNode, Environment& env);
pointer<Values::Runtime> IIf(pointer<AST::ExprAST>& astNode, Environment& env);
bool truthy(pointer<Values::Runtime>& conditional);

void runCommand(const string& command) {
	system(command.c_str());
}

std::map<string, std::unique_ptr<Values::Runtime>> ErrorStuff;
std::map<string, std::unique_ptr<Values::Runtime>> ConsoleStuff;
std::map<string, std::unique_ptr<Values::Runtime>> FileStuff;
Console console;
bool init = false;

namespace Interpreter {
	pointer<Values::Runtime> evaluate(pointer<AST::ExprAST>& astNode, Environment& env){
		ErrorStuff["throw"] = std::make_unique<NativeFN>(NativeFN(_throw));
		ConsoleStuff["out"] = std::make_unique<NativeFN>(NativeFN(thing));
	    ConsoleStuff["ask"] = std::make_unique<NativeFN>(NativeFN(ask));
		FileStuff["read"] = std::make_unique<NativeFN>(NativeFN(readFile));
	    FileStuff["write"] = std::make_unique<NativeFN>(NativeFN(writeFile));

		env.setVariableSafe("error", std::make_unique<Values::Object>(Values::Object(std::move(ErrorStuff))), true);
		env.setVariableSafe("console", std::make_unique<Values::Object>(Values::Object(std::move(ConsoleStuff))), true);
		env.setVariableSafe("file", std::make_unique<Values::Object>(Values::Object(std::move(FileStuff))), true);
		env.setVariableSafe("null", std::make_unique<Values::Null>(Values::Null()), true);
		env.setVariableSafe("true", std::make_unique<Values::Boolean>(Values::Boolean(true)), true);
		env.setVariableSafe("false", std::make_unique<Values::Boolean>(Values::Boolean(false)), true);

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
			case Nodes::If:
				return IIf(astNode, env);
			case Nodes::Skip:
				return makeptr<Values::Skip>(Values::Skip());
			case Nodes::ShellCmd:{
				AST::ShellCMD* cmd = dynamic_cast<AST::ShellCMD*>(astNode.get());
				runCommand(cmd->cmd);
				return makeptr<Values::Null>(Values::Null());
			} break;
			case Nodes::Break:
				return makeptr<Values::Break>(Values::Break());
			case Nodes::Function:
				return IFunction(astNode, env);
			case Nodes::Assignment:
				return IAssignment(astNode, env);
			break;
			case Nodes::While:
				return IWhile(astNode, env);
			case Nodes::Object:
				return IObject(astNode, env);
			break;
			case Nodes::Return: {
				AST::ReturnExpr* number = dynamic_cast<AST::ReturnExpr*>(astNode.get());
				return makeptr<Values::ReturnedValue>(Values::ReturnedValue(std::move(Interpreter::evaluate(number->value, env))));
			} break;
			default:
				std::cerr 
					<< "This AST Node is not yet set up for interpretation.\n" 
					<< "- Type: " << AST::stringifyAST(type) << "\n";
				exit(1);
		}
	}
}

pointer<Values::Runtime> INumber(pointer<AST::ExprAST>& astNode){
	AST::NumberExpr* number = dynamic_cast<AST::NumberExpr*>(astNode.get());
	return makeptr<Values::Number>(Values::Number(number->Value));
}
pointer<Values::Runtime> IIdent(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::Identifier* str = dynamic_cast<AST::Identifier*>(astNode.get());

	Variable val = env.getVariable(str->name);

	if(val.value != nullptr)
		return std::move(val.value);
	else
		throw std::runtime_error("Identifier is a null pointer");
}

void replaceAll(string &str, const string &from, const string &to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
}

pointer<Values::Runtime> IString(pointer<AST::ExprAST>& astNode){
	AST::StringExpr* str = dynamic_cast<AST::StringExpr*>(astNode.get());
	string val = str->Value;
	replaceAll(val, "\\n", "\n");
	replaceAll(val, "\\t", "\t");
	replaceAll(val, "\\b", "\b");

	return makeptr<Values::String>(Values::String(val));
}

pointer<Values::Runtime> IProgram(pointer<AST::ExprAST>& astNode, Environment& env) {
    string a = AST::stringifyAST(astNode.get()->getType());
	AST::Program* program = dynamic_cast<AST::Program*>(astNode.get());
    if (!program) {
        throw std::runtime_error("Invalid AST Node: Expected AST::Program.");
    }
    
    int i = 0;
    pointer<Values::Runtime> val;

    while (i < program->body.size()) {
        pointer<AST::ExprAST> expr = std::move(program->body[i]);
        val = Interpreter::evaluate(expr, env);
        i++;
    }

    return val;
}


pointer<Values::Runtime> IVariable(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::VariableExpr* var = dynamic_cast<AST::VariableExpr*>(astNode.get());
	pointer<Values::Runtime> val = Interpreter::evaluate(var->Value, env);

	if(val.get()->type() == var->Type || var->Type == "auto"){
		env.setVariable(var->Name, std::move(val), var->constant);
		return val;
	} else if(val->type() == "return"){
		Values::ReturnedValue* ret = dynamic_cast<Values::ReturnedValue*>(std::move(val.get()->clone().get()));
		env.setVariable(var->Name, std::move(ret->value.get()->clone()), var->constant);
		return std::move(ret->value);
	} else {
		std::cout << "Error while declaring variable '" << var->Name << "', because it seems like it's requiring type '" << var->Type << "' and the value applied to it was of type '" << val.get()->type() << "'.\n";
		exit(6);
	}
}

pointer<Values::Runtime> ICallFun(AST::CallExpr* call, Environment& env, std::vector<pointer<Values::Runtime>>& args){
	AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
	Variable fun = env.getVariable(ident->name);

	Values::Function* fn = dynamic_cast<Values::Function*>(std::move(fun.value.get()->clone().get()));
	pointer<Values::Runtime> value;

	int i = 0;
	for(auto& param : fn->parameters){
		if(args.size() >= i)
			env.setVariable(param, std::move(args[i]), false);
		i++;
	}
	
	for(auto& expr : fn->body){
		value = Interpreter::evaluate(expr, env);
		if(value.get()->type() == "return"){
			Values::ReturnedValue* sum = dynamic_cast<Values::ReturnedValue*>(std::move(value.get()->clone().get()));
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

pointer<Values::Runtime> ICallMem(Values::Function* fn, Environment& env, std::vector<pointer<Values::Runtime>>& args){
	pointer<Values::Runtime> value;

	int i = 0;
	for(auto& param : fn->parameters){
		if(args.size() >= i)
			env.setVariable(param, std::move(args[i]), false);
		else
			env.setVariable(param, makeptr<Values::Null>(Values::Null()), false);
		i++;
	}
	
	for(auto& expr : fn->body){
		value = Interpreter::evaluate(expr, env);
		if(value.get()->type() == "return"){
			Values::ReturnedValue* sum = dynamic_cast<Values::ReturnedValue*>(std::move(value.get()->clone().get()));
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

pointer<Values::Runtime> ICallMemNative(NativeFN* fn, Environment& env, std::vector<pointer<Values::Runtime>>& args){
	pointer<Values::Runtime> value = fn->call(std::move(args), env);

	return value;
}

pointer<Values::Runtime> ICall(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::CallExpr* call = dynamic_cast<AST::CallExpr*>(astNode.get());
	
	std::vector<pointer<Values::Runtime>> args{};
	for(auto& arg : call->Args){
		args.push_back(Interpreter::evaluate(arg, env));
	}
	Environment enva; enva.setParent(&env);

	Nodes calleeType = call->Callee.get()->getType();
	if(calleeType == Nodes::Member){
			pointer<Values::Runtime> value;
			pointer<Values::Runtime> val = IMember(call->Callee, enva);
//			console.log(val.get()->type());
			if(val.get()->type() == "function"){
				Values::Function* sym = dynamic_cast<Values::Function*>(std::move(val.get()->clone().get()));
				return ICallMem(sym, enva, args);
			} else if(val.get()->type() == "native-fn"){
				NativeFN* sym = dynamic_cast<NativeFN*>(std::move(val.get()->clone().get()));
				return ICallMemNative(sym, enva, args);
			}
//			else if(val.get()->type() != "proto-fn")
			else
				throw std::runtime_error("Callee is not a function");
	}
	
	
	AST::Identifier* ident = dynamic_cast<AST::Identifier*>(std::move(call->Callee.get()));
	pointer<Values::Runtime> fun = env.getVariable(ident->name).value;
	if(fun.get()->type() == "native-fn"){
		NativeFN* fn = dynamic_cast<NativeFN*>(std::move(fun.get()));
		return fn->call(std::move(args), env);
	} else if(fun.get()->type() == "function"){
		return ICallFun(call, env, args);
	}

	throw std::runtime_error("Non function is trying to get called.");
}

pointer<Values::Runtime> IArray(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::Array* arr = dynamic_cast<AST::Array*>(astNode.get());
	std::vector<pointer<Values::Runtime>> array{};

	for(auto& val : arr->elements){
		pointer<Values::Runtime> v = Interpreter::evaluate(val, env);
		array.push_back(std::move(v));
	}

	return makeptr<Values::Array>(Values::Array(std::move(array)));
}

pointer<Values::Runtime> IObject(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::Object* obj = dynamic_cast<AST::Object*>(astNode.get());
	std::map<string, pointer<Values::Runtime>> props;

	for (const auto& val : obj->map) {
		AST::Element* element = dynamic_cast<AST::Element*>(std::move(val.get()));
		props[element->key] = (Interpreter::evaluate(element->value, env));
	}

	return makeptr<Values::Object>(Values::Object(std::move(props)));
}

pointer<Values::Runtime> IMember(pointer<AST::ExprAST>& astNode, Environment& env){
	if(astNode == nullptr){
		std::cout << "astNode in IMember is a null pointer jsyk\n";
	}
	AST::MemberExpr* mem = dynamic_cast<AST::MemberExpr*>(astNode.get());
	pointer<Values::Runtime> val = Interpreter::evaluate(mem->object, env);
	if(val == nullptr){
		std::cout << "value coming from IMember is a null pointer jsyk\n";
	}
	AST::Identifier* id = dynamic_cast<AST::Identifier*>(mem->property.get()->clone().get());
	string sym = id->name;
	console.log(val.get()->type());
	if(val.get()->type() == "object"){
		Values::Object* obj = dynamic_cast<Values::Object*>(std::move(val.get()));
		pointer<Values::Runtime> value = std::move(obj->props[sym]);

		if(value != nullptr){
			return value;
		} else {
			std::cout << "Property " << sym << " doesn't exist on the object" << ".\n";
			exit(2);
		}
	} else {
		if(val.get()->type() == "string"){
			string value = dynamic_cast<Values::String*>(val.get())->value;
			return val;
		}
	}
	throw std::runtime_error("Cannot access property on non-object value");
}

pointer<Values::Runtime> IAssignment(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::AssignmentExpr* assignment = dynamic_cast<AST::AssignmentExpr*>(astNode.get());
	if(assignment->assignee.get()->getType() != AST::Nodes::Identifier){
		std::cout << "Invalid assignment.\n";
	}
	AST::Identifier* var = dynamic_cast<AST::Identifier*>(astNode.get());
	Variable vall = env.getVariable(var->name);
	string valueType = vall.value.get()->type();
	pointer<Values::Runtime> val = Interpreter::evaluate(assignment->value, env); 
	if(valueType == val.get()->type()){
		env.setVariable(var->name, std::move(val), vall.constant);
		return std::move(val);
	}
	else {
		std::cout << "Error while declaring variable '" << var->name << "', because it seems like it's requiring type '" << valueType << "' and the value applied to it was of type '" << val.get()->type() << "'.\n";
		exit(6);
	}
}

pointer<Values::Runtime> IBinary(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::BinaryExpr* binexp = dynamic_cast<AST::BinaryExpr*>(astNode.get());

	pointer<Values::Runtime> lhs = Interpreter::evaluate(binexp->LHS, env);
	pointer<Values::Runtime> rhs = Interpreter::evaluate(binexp->RHS, env);

	string lhsType = lhs.get()->type();
	string rhsType = rhs.get()->type();

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

		return makeptr<Values::Number>(Values::Number(result));

	} else if(lhs.get()->type() == "null" || lhs.get()->type() == "null"){
			return makeptr<Values::String>(Values::String("null"));
	} else {
		string lhsType = lhs.get()->type();
		string rhsType = rhs.get()->type();
		string value = "";
		if((lhsType != "function" || lhsType != "object") && (rhsType != "function" || rhsType != "object")){
			value.append(lhs.get()->stringValue()).append(rhs.get()->stringValue());
		} else {
			value = "null";
		}
		return makeptr<Values::String>(Values::String(value));
	}

	return makeptr<Values::Null>(Values::Null());
}

pointer<Values::Runtime> IIf(pointer<AST::ExprAST>& astNode, Environment& env){
	bool truu = true;
	pointer<Values::Runtime> val;
	AST::IfStatement* whilee = dynamic_cast<AST::IfStatement*>(astNode.get());
	Environment enva;
	enva.setParent(&env);
	pointer<Values::Runtime> cond = Interpreter::evaluate(whilee->conditional, enva);
	if(truthy(cond)){
		for(auto& thing : whilee->consequent){
			val = Interpreter::evaluate(thing, enva);
			if(val.get()->type() == "skip"){
				break;
			}
		}
	} else {
		if(whilee->alternate.size() > 0){
			for(auto& thing : whilee->alternate){
				val = Interpreter::evaluate(thing, enva);
				if(val.get()->type() == "skip"){
					break;
				}
			}
		}
	}
	return makeptr<Values::Null>(Values::Null());
}

pointer<Values::Runtime> IFunction(pointer<AST::ExprAST>& astNode, Environment& env){
	AST::Function* fun = dynamic_cast<AST::Function*>(astNode.get());

	env.setVariable(fun->name, makeptr<Values::Function>(Values::Function(std::move(fun->body), fun->params, fun->name, fun->type)), true);

	return env.getVariable(fun->name).value;
}

pointer<Values::Runtime> IWhen(pointer<AST::ExprAST>& astNode, Environment& env){
	return makeptr<Values::Null>(Values::Null());
}

pointer<Values::Runtime> IWhile(pointer<AST::ExprAST>& astNode, Environment& env){
	bool truu = true;
	pointer<Values::Runtime> val;
	AST::WhileDeclaration* whilee = dynamic_cast<AST::WhileDeclaration*>(astNode.get());
	Environment enva;
	enva.setParent(&env);
	for(int i = 0; i < 9999; i++){
		pointer<Values::Runtime> cond = Interpreter::evaluate(whilee->conditional, enva);
		if(truthy(cond)){
			for(auto& thing : whilee->consequent){
				val = Interpreter::evaluate(thing, enva);
				if(val.get()->type() == "break"){
					break;
				}
			}
		} else {
			truu = false;
		}
	}
	return val;
}

pointer<Values::Runtime> IDSN(pointer<AST::ExprAST>& astNode, Environment& env){
	return makeptr<Values::Null>(Values::Null());
}

string value(pointer<AST::ExprAST>& astNode){
	return "";
}
/*
function isTruthy(conditional: Runtime) {
    if (conditional.type == 'boolean') {
        const bool = (conditional as BooleanValue).value
        if (bool) return true
        else return false
    }

    if (conditional) {
        return true
    } else {
        return false
    }
}
*/

bool truthy(pointer<Values::Runtime>& conditional){
	if(conditional.get()->type() == "boolean"){
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