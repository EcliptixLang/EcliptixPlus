#ifndef ECLIPTIX_ENVIRONMENT_H
#define ECLIPTIX_ENVIRONMENT_H

#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <Values.h>

namespace Ecliptix {

    class Environment {
    public:
        explicit Environment(std::shared_ptr<Environment> parentENV = nullptr);

        std::shared_ptr<Ecliptix::Values::RuntimeValue> declareVar(const std::string& varname, std::shared_ptr<Ecliptix::Values::RuntimeValue> value);

        std::shared_ptr<Ecliptix::Values::RuntimeValue> assignVar(const std::string& varname, std::shared_ptr<Ecliptix::Values::RuntimeValue> value);

        std::shared_ptr<Ecliptix::Values::RuntimeValue> lookupVar(const std::string& varname);

    private:
        std::shared_ptr<Environment> resolve(const std::string& varname);

        std::shared_ptr<Environment> parent;

        std::unordered_map<std::string, std::shared_ptr<Ecliptix::Values::RuntimeValue>> variables;
    };

} // namespace Ecliptix

#endif // ECLIPTIX_ENVIRONMENT_H
