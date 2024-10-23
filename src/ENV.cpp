#include <ENV.hpp>
#include <FunctionValues.hpp>
#include <iostream>
#include <windows.h>
#include <winutils.hpp>

std::unique_ptr<Values::Runtime> thing(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
		std::cout << "hello world\n";
}

Environment::Environment(std::unique_ptr<Environment> parentENV)
        : parent(std::move(parentENV)) {
            variables["hi"] = std::make_unique<NativeFN>(NativeFN(thing));
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

                        std::cout << variables[var.first].get()->type() << std::endl;
                    }
                }
        	}

            for(auto& var : variables){
                std::cout << var.first << "\n";
            }
        }

std::unique_ptr<Values::Runtime> Environment::declareVar(const std::string& varname, std::unique_ptr<Values::Runtime> value, bool constant) {
    if (variables.find(varname) == variables.end()) {
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
    std::cout << it->first << std::endl;
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
