#include <ENV.hpp>
#include <FunctionValues.hpp>
#include <windows.h>

using Nodes = AST::Nodes; 
using string = std::string;

std::unique_ptr<Values::Runtime> thing(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value << "\n";

    return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> thingn(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value;

    return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> ask(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    thingn(std::move(args), env);
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

    std::string filecont = Utilities::readFile(args[0]->stringValue());

    return std::make_unique<Values::String>(Values::String(filecont));
}

std::unique_ptr<Values::Runtime> writeFile(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    if(args[0]->type() != "string"){
        std::cout << "Cannot read a non string\n";
        exit(6);
    }
    if(args[1]->type() != "string"){
        std::cout << "Cannot read a non string\n";
        exit(6);
    }
    
    Utilities::writeFile(args[0]->stringValue(), args[1]->stringValue());

    return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> holt(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    if(args[0]->type() != "number"){
        std::cout << "Cannot read a non number\n";
        exit(6);
    }

    Sleep(dynamic_cast<Values::Number*>(args[0].get())->value * 1000);

    return std::make_unique<Values::Null>(Values::Null());
}

std::unique_ptr<Values::Runtime> exeet(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    if(args[0]->type() != "number"){
        std::cout << "Cannot read a non number\n";
        exit(6);
    }

    exit(dynamic_cast<Values::Number*>(args[0].get())->value);
}

std::unique_ptr<Values::Runtime> errout(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cerr << value << "\n";

    return std::make_unique<Values::Null>(Values::Null());
}

std::map<std::string, std::unique_ptr<Values::Runtime>> ErrorStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> ConsoleStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> FileStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> ProcessStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> ProjectStuff;
std::map<std::string, std::unique_ptr<Values::Runtime>> AppStuff;

void Environment::setup(){
    ErrorStuff["throw"] = std::make_unique<NativeFN>(NativeFN(_throw));
    ErrorStuff["out"] = std::make_unique<NativeFN>(NativeFN(errout));
	ConsoleStuff["out"] = std::make_unique<NativeFN>(NativeFN(thing));
    ConsoleStuff["ask"] = std::make_unique<NativeFN>(NativeFN(ask));
	FileStuff["read"] = std::make_unique<NativeFN>(NativeFN(readFile));
    FileStuff["write"] = std::make_unique<NativeFN>(NativeFN(writeFile));
    ProcessStuff["exit"] = std::make_unique<NativeFN>(NativeFN(exeet));
    ProcessStuff["wait"] = std::make_unique<NativeFN>(NativeFN(holt));

	this->setVariableSafe("error", std::make_unique<Values::Object>(Values::Object(std::move(ErrorStuff))), true);
	this->setVariableSafe("console", std::make_unique<Values::Object>(Values::Object(std::move(ConsoleStuff))), true);
	this->setVariableSafe("file", std::make_unique<Values::Object>(Values::Object(std::move(FileStuff))), true);
    this->setVariableSafe("process", std::make_unique<Values::Object>(Values::Object(std::move(ProcessStuff))), true);
	this->setVariableSafe("null", std::make_unique<Values::Null>(Values::Null()), true);
	this->setVariableSafe("true", std::make_unique<Values::Boolean>(Values::Boolean(true)), true);
	this->setVariableSafe("false", std::make_unique<Values::Boolean>(Values::Boolean(false)), true);
}