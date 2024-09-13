#pragma once
#include "Values.h"
#include "AST.h"
#include <memory>

namespace Ecliptix::Interpreter {
    std::unique_ptr<Ecliptix::Values::RuntimeValue> evaluate(Ecliptix::AST::Statement astNode);
}
