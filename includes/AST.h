#pragma once
#include <string>
#include <vector>
#include <memory>

#define StmtArr std::vector<std::shared_ptr<Statement>>

namespace Ecliptix::AST {
    enum class NodeType {
        Program,
		VarDeclaration,
		Property,
		Element,
		IfStatement,
		WhileStatement,
		FunctionDeclaration,
		EqualityExpression,
		AssignmentExpression,
		MemberExpression,
		ArrayLiteral,
		StringLiteral,
		WhenDeclaration,
		DollarSignNotation,
		ObjectLiteral,
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

	struct StringLiteral : public Expression {
		std::string value;

		StringLiteral(NodeType kind, std::string value) : value(value) {
			this->kind = kind;
		}
	};

	struct ArrayElement : public Expression {
	    int index;
    	std::shared_ptr<Expression> value;

	    ArrayElement(int index, std::shared_ptr<Expression> value)
    	    : index(index), value(value) {
	        this->kind = NodeType::Element;
    	}
	};

	struct ArrayLiteral : public Expression {
		std::vector<ArrayElement> elements;

		ArrayLiteral()
	}

	struct NullLiteral : public Expression {

    	NullLiteral(NodeType kind) {
	        this->kind = kind;
    	}
	};

	struct BinaryExpression : public Expression {
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;
		std::string _operator;
	};


    std::string StringifyNodeTypes(NodeType type);
}
