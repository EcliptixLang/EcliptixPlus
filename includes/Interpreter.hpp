#pragma once
#include "Values.hpp"
#include "AST.hpp"
#include <ENV.hpp>
#include <memory>

namespace Interpreter {
    std::unique_ptr<Values::Runtime> evaluate(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
}
