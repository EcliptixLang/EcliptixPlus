#pragma once
#include <string>
#include "AST.h"
#include <Values.h>

namespace Ecliptix::Generators {
    std::unique_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol);
    std::unique_ptr<Ecliptix::AST::NumericLiteral> createNumber(std::string num);
	std::unique_ptr<Ecliptix::AST::NullLiteral> createNull();
	std::unique_ptr<Ecliptix::Values::NumberValue> createNumberValue(int num);
	std::unique_ptr<Ecliptix::Values::NullValue> createNullValue();
}
