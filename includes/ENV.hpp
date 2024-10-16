#ifndef ECLIPTIX_ENVIRONMENT_H
#define ECLIPTIX_ENVIRONMENT_H

#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <Values.hpp>

class Environment {
    public:
        explicit Environment(std::unique_ptr<Environment> parentENV = nullptr);

        std::unique_ptr<Values::Runtime> declareVar(const std::string& varname, std::unique_ptr<Values::Runtime> value);

        std::unique_ptr<Values::Runtime> assignVar(const std::string& varname, std::unique_ptr<Values::Runtime> value);

        std::unique_ptr<Values::Runtime> lookupVar(const std::string& varname);

    private:
        std::unique_ptr<Environment> resolve(const std::string& varname);

        std::unique_ptr<Environment> parent;

        std::unordered_map<std::string, std::unique_ptr<Values::Runtime>> variables;
};

#endif // ECLIPTIX_ENVIRONMENT_H
