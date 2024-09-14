#pragma once
#include "Values.h"
#include "AST.h"
#include <ENV.h>
#include <memory>

namespace Ecliptix::Interpreter {
    std::shared_ptr<Ecliptix::Values::RuntimeValue> evaluate(std::shared_ptr<Ecliptix::AST::Statement> astNode, Ecliptix::Environment env);
}
