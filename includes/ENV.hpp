#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>
#include <Values.hpp>

struct Variable {
    std::string name;
    std::unique_ptr<Values::Runtime> value;
    bool constant;
};

class Environment {
    public:
        Environment* getParent();
        int variableCount();
        void setParent(Environment* Parent);
        Variable getVariable(const std::string& varname);
        int parentCount(int num = 0);
        void setVariableSafe(const std::string& varname, std::unique_ptr<Values::Runtime> vallo, bool constant = false);
        void setVariable(const std::string& varname, std::unique_ptr<Values::Runtime> vallo, bool constant = false);
    private:
    Environment* parent = nullptr;
    std::vector<Variable> variables;
};

typedef void(*create)(Environment&);