#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>
#include <Values.hpp>

class Environment {
    public:
        explicit Environment(std::unique_ptr<Environment> parentENV = nullptr);

        std::unique_ptr<Values::Runtime> declareVar(const std::string& varname, std::unique_ptr<Values::Runtime> value, bool constant);

        std::unique_ptr<Values::Runtime> assignVar(const std::string& varname, std::unique_ptr<Values::Runtime> value);

        std::unique_ptr<Values::Runtime> lookupVar(const std::string& varname);
        std::vector<std::string> constants;
        std::unordered_map<std::string, std::unique_ptr<Values::Runtime>> variables;

    private:
        Environment* resolve(const std::string& varname);

        std::unique_ptr<Environment> parent;
};

typedef void(*create)(Environment&);