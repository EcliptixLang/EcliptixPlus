#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AST.h"
#include <Lexer.h>
#include <Values.h>

namespace Ecliptix::Generators {
    // Function prototypes for creating AST nodes
    std::shared_ptr<Ecliptix::AST::IdentifierLiteral> createIdent(std::string symbol);
    std::shared_ptr<Ecliptix::AST::NumericLiteral> createNumber(int value);  // Changed from string to int
    std::shared_ptr<Ecliptix::AST::StringLiteral> createString(std::string value);
    std::shared_ptr<Ecliptix::AST::NullLiteral> createNull();
    std::shared_ptr<Ecliptix::AST::ArrayElement> createArrayElement(int index, std::shared_ptr<Ecliptix::AST::Expression> value);
    std::shared_ptr<Ecliptix::AST::BinaryExpression> createBinaryExpr(std::shared_ptr<Ecliptix::AST::Expression> left,
                                                                      std::shared_ptr<Ecliptix::AST::Expression> right,
                                                                      std::string _operator);
    std::shared_ptr<Ecliptix::AST::AssignmentExpression> createAssignmentExpr(std::shared_ptr<Ecliptix::AST::Expression> assigne,
                                                                            std::shared_ptr<Ecliptix::AST::Expression> value);
    std::shared_ptr<Ecliptix::AST::CallExpression> createCallExpr(std::vector<std::shared_ptr<Ecliptix::AST::Expression>> args,
                                                                  std::shared_ptr<Ecliptix::AST::Expression> caller);
    std::shared_ptr<Ecliptix::AST::MemberExpression> createMemberExpr(std::shared_ptr<Ecliptix::AST::Expression> object,
                                                                      std::shared_ptr<Ecliptix::AST::Expression> property,
                                                                      bool computed);
    std::shared_ptr<Ecliptix::AST::Property> createProperty(std::string key, std::shared_ptr<Ecliptix::AST::Expression> value);
    std::shared_ptr<Ecliptix::AST::ObjectLiteral> createObjectLiteral(std::vector<std::shared_ptr<Ecliptix::AST::Property>> properties);
    std::shared_ptr<Ecliptix::AST::ArrayLiteral> createArrayLiteral(std::vector<std::shared_ptr<Ecliptix::AST::ArrayElement>> elements);
    std::shared_ptr<Ecliptix::AST::FunctionDeclaration> createFunctionDeclaration(std::vector<std::string> parameters,
                                                                                  std::string name,
                                                                                  StmtArr body,
                                                                                  Ecliptix::AST::NodeType returnType);
    std::shared_ptr<Ecliptix::AST::IfStatement> createIfStatement(std::shared_ptr<Ecliptix::AST::Expression> conditional,
                                                                  Ecliptix::Lexer::TokenType operatorType,
                                                                  StmtArr consequent,
                                                                  std::optional<StmtArr> alternate);
    std::shared_ptr<Ecliptix::AST::WhileStatement> createWhileStatement(std::shared_ptr<Ecliptix::AST::Expression> conditional,
                                                                        Ecliptix::Lexer::TokenType operatorType,
                                                                        StmtArr body);
    std::shared_ptr<Ecliptix::AST::WhenDeclaration> createWhenDeclaration(std::shared_ptr<Ecliptix::AST::Expression> conditional,
                                                                          Ecliptix::Lexer::TokenType operatorType,
                                                                          StmtArr consequent);
    std::shared_ptr<Ecliptix::AST::DollarSignNotation> createDollarSignNotation(std::shared_ptr<Ecliptix::AST::StringLiteral> shellCmd);
	std::shared_ptr<Ecliptix::AST::VarDeclaration> createVarDeclaration(bool constant, std::string identifier, std::shared_ptr<Ecliptix::AST::Expression> value);
    // Function prototypes for creating Value objects
    std::shared_ptr<Ecliptix::Values::NumberValue> createNumberValue(int num);
    std::shared_ptr<Ecliptix::Values::NullValue> createNullValue();
}
