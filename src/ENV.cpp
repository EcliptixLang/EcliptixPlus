#include <ENV.hpp>
#include <FunctionValues.hpp>
#include <iostream>
#include <windows.h>
#include <winutils.hpp>
#include <Utilities.hpp>

Environment* Environment::getParent(){
    return parent;
}

int Environment::variableCount(){
    return variables.size();
}

void Environment::setParent(Environment* Parent){
    parent = Parent;
}

Variable Environment::getVariable(const std::string& varname){
    for(auto& var: variables){
        if(var.name == varname)
            return std::move(var);
    }

    if(!parent){
        std::cout << "Variable " << varname << " undefined, parent count: " << parentCount() << "\n";
        exit(1);
    }

    return parent->getVariable(varname);
}

int Environment::parentCount(int num){
    num++;
    if(!parent)
        return num;

    return parent->parentCount(num);
}

void Environment::setVariable(const std::string& varname, std::unique_ptr<Values::Runtime> vallo, bool constant){
    for (auto& var : variables) {
        if (var.name == varname) {
            if (var.constant) {
                std::cout << "Cannot modify a constant variable.\n";
                return;
            }
            var.value = std::move(vallo);
            return;
        }
    }
    variables.push_back({ varname, std::move(vallo), constant });
}

void Environment::setVariableSafe(const std::string& varname, std::unique_ptr<Values::Runtime> vallo, bool constant){
    for (auto& var : variables) {
        if (var.name == varname) {
            if (var.constant) {
                return;
            }
            var.value = std::move(vallo);
            return;
        }
    }
    variables.push_back({ varname, std::move(vallo), constant });
}

/*
void Environment::Initialize(){
    
    constants.push_back("false");
}*/