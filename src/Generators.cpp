#include <Generators.h>
#include <Values.h>
#include <AST.h>
#include <Lexer.h>
#include <iostream>

#define StmtArr std::vector<std::shared_ptr<Ecliptix::AST::Statement>>

namespace Ecliptix::Generators {

    std::shared_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol) {
        return std::make_shared<Ecliptix::AST::IdentifierLiteral>(std::move(symbol));
    }

    std::shared_ptr<Ecliptix::AST::NumericLiteral> createNumber(int value) {
        return std::make_shared<Ecliptix::AST::NumericLiteral>(value);
    }

    std::shared_ptr<Ecliptix::AST::StringLiteral> createString(std::string value) {
        return std::make_shared<Ecliptix::AST::StringLiteral>(std::move(value));
    }

    std::shared_ptr<Ecliptix::AST::NullLiteral> createNull() {
        return std::make_shared<Ecliptix::AST::NullLiteral>();
    }

    std::shared_ptr<Ecliptix::AST::ArrayElement> createArrayElement(int index, std::shared_ptr<Ecliptix::AST::Expression> value) {
        return std::make_shared<Ecliptix::AST::ArrayElement>(index, std::move(value));
    }

    std::shared_ptr<Ecliptix::AST::BinaryExpression> createBinaryExpr(std::shared_ptr<Ecliptix::AST::Expression> left,
                                                                      std::shared_ptr<Ecliptix::AST::Expression> right,
                                                                      std::string _operator) {
        return std::make_shared<Ecliptix::AST::BinaryExpression>(std::move(left), std::move(right), std::move(_operator));
    }

    std::shared_ptr<Ecliptix::AST::AssignmentExpression> createAssignmentExpr(std::shared_ptr<Ecliptix::AST::Expression> assigne,
                                                                            std::shared_ptr<Ecliptix::AST::Expression> value) {
        return std::make_shared<Ecliptix::AST::AssignmentExpression>(std::move(assigne), std::move(value));
    }

    std::shared_ptr<Ecliptix::AST::CallExpression> createCallExpr(std::vector<std::shared_ptr<Ecliptix::AST::Expression>> args,
                                                                  std::shared_ptr<Ecliptix::AST::Expression> caller) {
        return std::make_shared<Ecliptix::AST::CallExpression>(std::move(args), std::move(caller));
    }

    std::shared_ptr<Ecliptix::AST::MemberExpression> createMemberExpr(std::shared_ptr<Ecliptix::AST::Expression> object,
                                                                      std::shared_ptr<Ecliptix::AST::Expression> property,
                                                                      bool computed) {
        return std::make_shared<Ecliptix::AST::MemberExpression>(std::move(object), std::move(property), computed);
    }

    std::shared_ptr<Ecliptix::AST::Property> createProperty(std::string key, std::shared_ptr<Ecliptix::AST::Expression> value) {
        return std::make_shared<Ecliptix::AST::Property>(std::move(key), std::move(value));
    }

    std::shared_ptr<Ecliptix::AST::ObjectLiteral> createObjectLiteral(std::vector<std::shared_ptr<Ecliptix::AST::Property>> properties) {
        return std::make_shared<Ecliptix::AST::ObjectLiteral>(std::move(properties));
    }

    std::shared_ptr<Ecliptix::AST::ArrayLiteral> createArrayLiteral(std::vector<std::shared_ptr<Ecliptix::AST::ArrayElement>> elements) {
        return std::make_shared<Ecliptix::AST::ArrayLiteral>(std::move(elements));
    }

    std::shared_ptr<Ecliptix::AST::FunctionDeclaration> createFunctionDeclaration(std::vector<std::string> parameters,
                                                                                  std::string name,
                                                                                  StmtArr body,
                                                                                  Ecliptix::AST::NodeType returnType) {
        return std::make_shared<Ecliptix::AST::FunctionDeclaration>(std::move(parameters), std::move(name), std::move(body), returnType);
    }

    std::shared_ptr<Ecliptix::AST::IfStatement> createIfStatement(std::shared_ptr<Ecliptix::AST::Expression> conditional,
                                                                  Ecliptix::Lexer::TokenType operatorType,
                                                                  StmtArr consequent,
                                                                  std::optional<StmtArr> alternate) {
        return std::make_shared<Ecliptix::AST::IfStatement>(std::move(conditional), operatorType, std::move(consequent), std::move(alternate));
    }

    std::shared_ptr<Ecliptix::AST::WhileStatement> createWhileStatement(std::shared_ptr<Ecliptix::AST::Expression> conditional,
                                                                        Ecliptix::Lexer::TokenType operatorType,
                                                                        StmtArr body) {
        return std::make_shared<Ecliptix::AST::WhileStatement>(std::move(conditional), operatorType, std::move(body));
    }

    std::shared_ptr<Ecliptix::AST::WhenDeclaration> createWhenDeclaration(std::shared_ptr<Ecliptix::AST::Expression> conditional,
                                                                          Ecliptix::Lexer::TokenType operatorType,
                                                                          StmtArr consequent) {
        return std::make_shared<Ecliptix::AST::WhenDeclaration>(std::move(conditional), operatorType, std::move(consequent));
    }

std::shared_ptr<Ecliptix::AST::VarDeclaration> createVarDeclaration(bool constant, std::string identifier, std::shared_ptr<Ecliptix::AST::Expression> value)	{
		return std::make_shared<Ecliptix::AST::VarDeclaration>(constant, identifier, std::move(value));
	}

    std::shared_ptr<Ecliptix::AST::DollarSignNotation> createDollarSignNotation(std::shared_ptr<Ecliptix::AST::StringLiteral> shellCmd) {
        return std::make_shared<Ecliptix::AST::DollarSignNotation>(std::move(shellCmd));
    }

	std::shared_ptr<Ecliptix::Values::NumberValue> createNumberValue(int num) {
    	return std::make_shared<Ecliptix::Values::NumberValue>(num);
	}


    std::shared_ptr<Ecliptix::Values::NullValue> createNullValue(){
        return std::make_shared<Ecliptix::Values::NullValue>();
    }

    // Example usage
    void testGenerators() {
        auto ident = createIdent("myVariable");
        auto num = createNumber(42);
        auto str = createString("hello");
        auto null = createNull();
        auto arrayElem = createArrayElement(0, ident);
        auto binExpr = createBinaryExpr(ident, num, "+");
        auto assignExpr = createAssignmentExpr(ident, num);
        auto callExpr = createCallExpr({ident, num}, ident);
        auto memberExpr = createMemberExpr(ident, ident, true);
        auto property = createProperty("key", str);
        auto objLiteral = createObjectLiteral({property});
        auto arrayLiteral = createArrayLiteral({arrayElem});
        auto funcDecl = createFunctionDeclaration({"param1", "param2"}, "myFunction", {assignExpr}, Ecliptix::AST::NodeType::FunctionDeclaration);
        auto ifStmt = createIfStatement(ident, Ecliptix::Lexer::TokenType::BinaryEquals, {assignExpr}, std::nullopt);
        auto whileStmt = createWhileStatement(ident, Ecliptix::Lexer::TokenType::BinaryEquals, {assignExpr});
        auto whenDecl = createWhenDeclaration(ident, Ecliptix::Lexer::TokenType::BinaryEquals, {assignExpr});
        auto dollarNotation = createDollarSignNotation(str);
    }

}