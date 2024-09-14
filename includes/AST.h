#pragma once
#include <string>
#include <vector>
#include <memory>

#define StmtArr std::vector<std::unique_ptr<Statement>>

namespace Ecliptix::AST {
    enum class NodeType {
        Program,
        NumericLiteral,
		NullLiteral,
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

		Program(NodeType kind, StmtArr body) : body(std::move(body)) {
			this->kind = kind;
		}
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

	struct NullLiteral : public Expression {

    	NullLiteral(NodeType kind) {
	        this->kind = kind;
    	}
	};

	struct BinaryExpression : public Expression {
		std::unique_ptr<Expression> left;
		std::unique_ptr<Expression> right;
		std::string _operator;
	};


    std::string StringifyNodeTypes(NodeType type);
}
