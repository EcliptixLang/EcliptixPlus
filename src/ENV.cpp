#include <ENV.h>
#include <Generators.h>

namespace Ecliptix {

    Environment::Environment(std::shared_ptr<Environment> parentENV)
        : parent(parentENV) {
        variables["hi"] = Ecliptix::Generators::createNumberValue(69);
    }

    std::shared_ptr<Ecliptix::Values::RuntimeValue> Environment::declareVar(const std::string& varname, std::shared_ptr<Ecliptix::Values::RuntimeValue> value) {
        if (variables.find(varname) != variables.end()) {
            throw std::runtime_error("Cannot declare variable " + varname + ". It is already defined.");
        }
        variables[varname] = value;
        return value;
    }

    std::shared_ptr<Ecliptix::Values::RuntimeValue> Environment::assignVar(const std::string& varname, std::shared_ptr<Ecliptix::Values::RuntimeValue> value) {
        std::shared_ptr<Environment> env = resolve(varname);
        env->variables[varname] = value;
        return value;
    }

    std::shared_ptr<Ecliptix::Values::RuntimeValue> Environment::lookupVar(const std::string& varname) {
        std::shared_ptr<Environment> env = resolve(varname);
        auto it = env->variables.find(varname);
        if (it != env->variables.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable " + varname + " not found.");
    }

    std::shared_ptr<Environment> Environment::resolve(const std::string& varname) {
        if (variables.find(varname) != variables.end()) {
            return std::make_shared<Environment>();
        }

        if (!parent) {
            throw std::runtime_error("Cannot resolve '" + varname + "' as it does not exist.");
        }

        return parent->resolve(varname);
    }

} // namespace Ecliptix
