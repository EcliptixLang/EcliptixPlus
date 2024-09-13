#pragma once
#include <string>
#include <vector>

#define StmtArr std::vector<Ecliptix::AST::Statement>

namespace Ecliptix::AST {
    enum class NodeType {
        Program,
        NumericLiteral,
        StringLiteral,
        Identifier,
        BinaryExpression,
        CallExpression,
        UnaryExpression,
        FunctionDeclaration
    };

    struct Statement {
        NodeType kind;
    };

    struct Program : public Statement {
        StmtArr body;
    };

    struct Expression : public Statement {};
    
    struct IdentifierLiteral : public Expression {
        std::string symbol;
    };

    struct NumericLiteral : public Expression {
        int value;
    };

    std::string StringifyNodeTypes(NodeType type);
}
