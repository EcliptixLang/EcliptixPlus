#include <ENV.hpp>

Environment::Environment(std::unique_ptr<Environment> parentENV)
        : parent(std::move(parentENV)) {}

std::unique_ptr<Values::Runtime> Environment::declareVar(const std::string& varname, std::unique_ptr<Values::Runtime> value) {
    if (variables.find(varname) != variables.end()) {
        throw std::runtime_error("Cannot declare variable " + varname + ". It is already defined.");
    }
    variables[varname] = std::move(value);
    return value;
}

std::unique_ptr<Values::Runtime> Environment::assignVar(const std::string& varname, std::unique_ptr<Values::Runtime> value) {
    std::unique_ptr<Environment> env = resolve(varname);
    env->variables[varname] = std::move(value);
    return value;
}

std::unique_ptr<Values::Runtime> Environment::lookupVar(const std::string& varname) {
    std::unique_ptr<Environment> env = resolve(varname);
    auto it = env->variables.find(varname);
    if (it != env->variables.end()) {
        return std::move(it->second);
    }
    throw std::runtime_error("Variable " + varname + " not found.");
}

    std::unique_ptr<Environment> Environment::resolve(const std::string& varname) {
        if (variables.find(varname) != variables.end()) {
            return std::make_unique<Environment>();
        }

        if (!parent) {
            throw std::runtime_error("Cannot resolve '" + varname + "' as it does not exist.");
        }

        return parent->resolve(varname);
    }
