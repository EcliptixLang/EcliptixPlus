#include <Generators.h>
#include <iostream>

namespace Ecliptix::Generators {
    Ecliptix::AST::IdentifierLiteral createIdent(std::string symbol) {
        return { Ecliptix::AST::NodeType::Identifier, symbol };
    }

    Ecliptix::AST::NumericLiteral createNumber(std::string num) {
        std::cout << num << " and " << std::stoi(num) << std::endl;
        return { Ecliptix::AST::NodeType::NumericLiteral, std::stoi(num) };
    }
}
