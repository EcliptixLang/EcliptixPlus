#pragma once
#include <string>
#include <vector>
#include <memory>

#define StmtArr std::vector<std::unique_ptr<Statement>>

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

		virtual ~Statement() = default;
    };

    struct Program : public Statement {
        StmtArr body;
    };

    struct Expression : public Statement {};
    
    struct IdentifierLiteral : public Expression {
		std::string symbol;

		IdentifierLiteral(NodeType kind, std::string symbol) : symbol(symbol) {
			this->kind = kind;
		}
	};

	struct NumericLiteral : public Expression {
	    int value;

    	NumericLiteral(NodeType kind, int value) : value(value) {
	        this->kind = kind;
    	}
	};

    std::string StringifyNodeTypes(NodeType type);
}
