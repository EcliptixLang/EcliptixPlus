#pragma once
#include <string>
#include "AST.h"
#include <Values.h>

namespace Ecliptix::Generators {
    std::shared_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol);
    std::shared_ptr<Ecliptix::AST::NumericLiteral> createNumber(std::string num);
	std::shared_ptr<Ecliptix::AST::NullLiteral> createNull();
	std::shared_ptr<Ecliptix::Values::NumberValue> createNumberValue(int num);
	std::shared_ptr<Ecliptix::Values::NullValue> createNullValue();
}
