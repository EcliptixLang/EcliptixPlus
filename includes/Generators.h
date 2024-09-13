#pragma once
#include <string>
#include "AST.h"

namespace Ecliptix::Generators {
    Ecliptix::AST::IdentifierLiteral createIdent(std::string symbol);
    Ecliptix::AST::NumericLiteral createNumber(std::string num);
}
