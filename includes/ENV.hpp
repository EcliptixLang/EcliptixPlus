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
    std::string type;
};

struct thingy {
    std::unique_ptr<AST::ExprAST> cond;
    std::vector<std::unique_ptr<AST::ExprAST>> consequent;
};

class Environment {
    public:
        void setup();
        Environment* getParent();
        int variableCount();
        void setParent(Environment* Parent);
        Variable getVariable(const std::string& varname);
        int parentCount(int num = 0);
        void setVariableSafe(const std::string& varname, std::unique_ptr<Values::Runtime> vallo, bool constant = false);
        void setVariable(const std::string& varname, std::unique_ptr<Values::Runtime> vallo, std::string type, bool constant = false);
        std::map<std::string, thingy> events;
    private:
    Environment* parent = nullptr;
    std::vector<Variable> variables;
};

typedef void(*create)(Environment&);