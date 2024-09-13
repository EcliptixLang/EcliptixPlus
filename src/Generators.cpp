#include <Generators.h>
#include <iostream>

namespace Ecliptix::Generators {
    std::unique_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol){
        return std::make_unique<Ecliptix::AST::IdentifierLiteral>(Ecliptix::AST::NodeType::Identifier, symbol);
    }

    std::unique_ptr<Ecliptix::AST::NumericLiteral> createNumber(std::string num){
        return std::make_unique<Ecliptix::AST::NumericLiteral>(Ecliptix::AST::NodeType::NumericLiteral, std::stoi(num));
    }
}