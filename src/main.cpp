#include <Parser.hpp>
#include <Interpreter.hpp>
#include <winutils.hpp>
#include <FunctionValues.hpp>

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
    thing(std::move(args), env);
    std::string ans;
    std::cin >> ans;
    
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

Parser parser;
Environment ParentENV;
std::map<std::string, std::unique_ptr<Values::Runtime>> ErrorStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> ConsoleStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> FileStuff;

int runFile(std::string filePath){
	std::string code = Utilities::readFile(filePath);

	std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
	Interpreter::evaluate(ast, ParentENV);
	
	return 1;
}

int runREPL(){
    while(true){
        std::cout << "> ";
        std::string code;
        std::getline(std::cin, code);
        if(code == "exit")
            exit(0);

        std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
        Interpreter::evaluate(ast, ParentENV);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    ErrorStuff["throw"] = std::make_unique<NativeFN>(NativeFN(_throw));
	ConsoleStuff["out"] = std::make_unique<NativeFN>(NativeFN(thing));
    ConsoleStuff["ask"] = std::make_unique<NativeFN>(NativeFN(ask));
	FileStuff["read"] = std::make_unique<NativeFN>(NativeFN(readFile));
    FileStuff["write"] = std::make_unique<NativeFN>(NativeFN(writeFile));

	ParentENV.setVariableSafe("error", std::make_unique<Values::Object>(Values::Object(std::move(ErrorStuff))), true);
	ParentENV.setVariableSafe("console", std::make_unique<Values::Object>(Values::Object(std::move(ConsoleStuff))), true);
	ParentENV.setVariableSafe("file", std::make_unique<Values::Object>(Values::Object(std::move(FileStuff))), true);
	ParentENV.setVariableSafe("null", std::make_unique<Values::Null>(Values::Null()), true);
	ParentENV.setVariableSafe("true", std::make_unique<Values::Boolean>(Values::Boolean(true)), true);
	ParentENV.setVariableSafe("false", std::make_unique<Values::Boolean>(Values::Boolean(false)), true);
	if(argc > 1)
		return runFile(argv[1]);
	else
		return runREPL();
}