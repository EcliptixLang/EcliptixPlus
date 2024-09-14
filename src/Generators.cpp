#include <Generators.h>
#include <Values.h>
#include <iostream>

namespace Ecliptix::Generators {
    std::shared_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol){
        return std::make_shared<Ecliptix::AST::IdentifierLiteral>(Ecliptix::AST::NodeType::Identifier, symbol);
    }

    std::shared_ptr<Ecliptix::AST::NumericLiteral> createNumber(std::string num){
        return std::make_shared<Ecliptix::AST::NumericLiteral>(Ecliptix::AST::NodeType::NumericLiteral, std::stoi(num));
    }

	std::shared_ptr<Ecliptix::AST::NullLiteral> createNull(){
        return std::make_shared<Ecliptix::AST::NullLiteral>(Ecliptix::AST::NodeType::NullLiteral);
    }

	/*std::shared_ptr<Ecliptix::Values::Identifier> createIdentValue(std::string symbol){
        return std::make_shared<Ecliptix::AST::IdentifierLiteral>(Ecliptix::AST::NodeType::Identifier, symbol);
    }*/

    std::shared_ptr<Ecliptix::Values::NumberValue> createNumberValue(int num) {
    	return std::make_shared<Ecliptix::Values::NumberValue>(num);
	}


    std::shared_ptr<Ecliptix::Values::NullValue> createNullValue(){
        return std::make_shared<Ecliptix::Values::NullValue>();
    }
}