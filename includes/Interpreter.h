#pragma once
#include <memory>
#include <Values.h>
#include <AST.h>

namespace Ecliptix::Interpreter {
	std::unique_ptr<Ecliptix::Values::RuntimeValue> evaluate(Ecliptix::AST::Statement astNode);
} // namespace Ecliptix::Interpreter
