#include <ENV.hpp>
#include <FunctionValues.hpp>
#include <iostream>
#include <windows.h>
#include <winutils.hpp>

std::unique_ptr<Values::Runtime> thing(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg.get()->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value << "\n";
    
    return std::make_unique<Values::String>(Values::String(value));
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
    std::cin >> ans;
    return std::make_unique<Values::String>(Values::String(ans));
}

std::unique_ptr<Values::Runtime> _throw(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    std::cout << "Error:\n- Code: 8\n- Description: ";
    thing(std::move(args), env);
    exit(8);

    return std::make_unique<Values::Null>(Values::Null());
}

Environment::Environment() {
            std::map<std::string, std::unique_ptr<Values::Runtime>> stringPrototypes;
            stringPrototypes["throw"] = std::make_unique<NativeFN>(NativeFN(_throw));
            variables["error"] = std::make_unique<Values::Object>(Values::Object(stringPrototypes));

            std::map<std::string, std::unique_ptr<Values::Runtime>> ConsoleStuff;
            ConsoleStuff["out"] = std::make_unique<NativeFN>(NativeFN(thing));
            ConsoleStuff["ask"] = std::make_unique<NativeFN>(NativeFN(ask));
            variables["console"] = std::make_unique<Values::Object>(Values::Object(ConsoleStuff));

            variables["null"] = std::make_unique<Values::Null>(Values::Null());
            std::vector<std::string> dlls = getDLLs();
        
        	for(auto& dll : dlls){
                HMODULE hModule = LoadLibraryA(dll.c_str());
                if (!hModule) {
                    DWORD errorMessageID = ::GetLastError();
                    std::cerr << "Could not load the \"" << dll << "\" Library.\n" << errorMessageID << std::endl;
                }

                typedef Environment (*CreateLibFn)();
    
                CreateLibFn createLibFunction = (CreateLibFn)GetProcAddress(hModule, "createLib");

                if (!createLibFunction) {
                    std::cerr << "Could not locate the \"createLib\" function for loading." << std::endl;
                    FreeLibrary(hModule);
                }

                Environment eenv = createLibFunction();

                for(auto& var : eenv.variables){
                    if(!variables[var.first]){
                        variables[var.first] = std::move(var.second);
                        constants.push_back(var.first);
                    }
                }
        	}
        }

std::unique_ptr<Values::Runtime> Environment::declareVar(const std::string& varname, std::unique_ptr<Values::Runtime> value, bool constant) {
    if (variables.find(varname) != variables.end()) {
        throw std::runtime_error("Cannot declare variable " + varname + ". It is already defined.");
    }
    variables[varname] = std::move(value);
    constants.push_back(varname);
    return value;
}

std::unique_ptr<Values::Runtime> Environment::assignVar(const std::string& varname, std::unique_ptr<Values::Runtime> value) {
    Environment* env = resolve(varname);
    env->variables[varname] = std::move(value);
    return value;
}

std::unique_ptr<Values::Runtime> Environment::lookupVar(const std::string& varname) {
    Environment* env = resolve(varname);
    auto it = env->variables.find(varname);
    if (it != env->variables.end()) {
        return std::move(it->second);
    }
    throw std::runtime_error("Variable " + varname + " not found.");
}

Environment* Environment::resolve(const std::string& varname) {
    // Check if the variable exists in the current environment
    if (variables.find(varname) != variables.end()) {
        return this; // Return a pointer to the current environment
    }

    // If there's no parent, the variable doesn't exist
    if (!parent) {
        throw std::runtime_error("Cannot resolve '" + varname + "' as it does not exist.");
    }

    // Otherwise, continue to resolve in the parent environment
    return parent->resolve(varname);
}

void Environment::setParent(Environment& env) {
    parent = &env;
}