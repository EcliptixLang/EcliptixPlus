#pragma once
#include <string>
#include "AST.h"

namespace Ecliptix::Generators {
    std::unique_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol);
    std::unique_ptr<Ecliptix::AST::NumericLiteral> createNumber(std::string num);
}
