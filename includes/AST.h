#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>

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

        IdentifierLiteral(std::string symbol) : symbol(std::move(symbol)) {
            this->kind = NodeType::Identifier;
        }
    };

    struct NumericLiteral : public Expression {
        int value;

        NumericLiteral(int value) : value(value) {
            this->kind = NodeType::NumericLiteral;
        }
    };

    struct StringLiteral : public Expression {
        std::string value;

        StringLiteral(std::string value) : value(std::move(value)) {
            this->kind = NodeType::StringLiteral;
        }
    };

	struct VarDeclaration : public Statement {
		VarDeclaration(bool constant, std::string identifier, std::shared_ptr<Expression> value) : constant(constant), identifier(identifier), value(std::move(value)) {
			this->kind = NodeType::VarDeclaration;
		}

		bool constant;
		std::string identifier;
		std::shared_ptr<Expression> value;
	};

    struct ArrayElement : public Expression {
        int index;
        std::shared_ptr<Expression> value;

        ArrayElement(int index, std::shared_ptr<Expression> value)
            : index(index), value(std::move(value)) {
            this->kind = NodeType::Element;
        }
    };

    struct NullLiteral : public Expression {
        NullLiteral() {
            this->kind = NodeType::NullLiteral;
        }
    };

    struct BinaryExpression : public Expression {
        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
        std::string _operator;

        BinaryExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, std::string _operator)
            : left(std::move(left)), right(std::move(right)), _operator(std::move(_operator)) {
            this->kind = NodeType::BinaryExpression;
        }
    };

    struct AssignmentExpression : public Expression {
        std::shared_ptr<Expression> assigne;
        std::shared_ptr<Expression> value;

        AssignmentExpression(std::shared_ptr<Expression> assigne, std::shared_ptr<Expression> value)
            : assigne(std::move(assigne)), value(std::move(value)) {
            this->kind = NodeType::AssignmentExpression;
        }
    };

    struct CallExpression : public Expression {
        std::vector<std::shared_ptr<Expression>> args;
        std::shared_ptr<Expression> caller;

        CallExpression(std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<Expression> caller)
            : args(std::move(args)), caller(std::move(caller)) {
            this->kind = NodeType::CallExpression;
        }
    };

    struct MemberExpression : public Expression {
        std::shared_ptr<Expression> object;
        std::shared_ptr<Expression> property;
        bool computed;

        MemberExpression(std::shared_ptr<Expression> object, std::shared_ptr<Expression> property, bool computed)
            : object(std::move(object)), property(std::move(property)), computed(computed) {
            this->kind = NodeType::MemberExpression;
        }
    };

    struct Property : public Expression {
        std::string key;
        std::shared_ptr<Expression> value;

        Property(std::string key, std::shared_ptr<Expression> value)
            : key(std::move(key)), value(std::move(value)) {
            this->kind = NodeType::Property;
        }
    };

    struct ObjectLiteral : public Expression {
        std::vector<Property> properties;

        ObjectLiteral(std::vector<Property> properties) : properties(std::move(properties)) {
            this->kind = NodeType::ObjectLiteral;
        }
    };

    struct ArrayLiteral : public Expression {
        std::vector<ArrayElement> elements;

        ArrayLiteral(std::vector<ArrayElement> elements) : elements(std::move(elements)) {
            this->kind = NodeType::ArrayLiteral;
        }
    };

    struct FunctionDeclaration : public Statement {
        std::vector<std::string> parameters;
        std::string name;
        StmtArr body;
        NodeType returnType;

        FunctionDeclaration(std::vector<std::string> parameters, std::string name, StmtArr body, NodeType returnType)
            : parameters(std::move(parameters)), name(std::move(name)), body(std::move(body)), returnType(returnType) {
            this->kind = NodeType::FunctionDeclaration;
        }
    };

    struct IfStatement : public Statement {
        std::shared_ptr<Expression> conditional;
        NodeType operatorType;
        StmtArr consequent;
        std::optional<StmtArr> alternate;

        IfStatement(std::shared_ptr<Expression> conditional, NodeType operatorType, StmtArr consequent, std::optional<StmtArr> alternate = std::nullopt)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)), alternate(std::move(alternate)) {
            this->kind = NodeType::IfStatement;
        }
    };

    struct WhileStatement : public Statement {
        std::shared_ptr<Expression> conditional;
        NodeType operatorType;
        StmtArr body;

        WhileStatement(std::shared_ptr<Expression> conditional, NodeType operatorType, StmtArr body)
            : conditional(std::move(conditional)), operatorType(operatorType), body(std::move(body)) {
            this->kind = NodeType::WhileStatement;
        }
    };

    struct WhenDeclaration : public Statement {
        std::shared_ptr<Expression> conditional;
        NodeType operatorType;
        StmtArr consequent;

        WhenDeclaration(std::shared_ptr<Expression> conditional, NodeType operatorType, StmtArr consequent)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)) {
            this->kind = NodeType::WhenDeclaration;
        }
    };

    struct DollarSignNotation : public Expression {
        std::shared_ptr<StringLiteral> shellCmd;

        DollarSignNotation(std::shared_ptr<StringLiteral> shellCmd)
            : shellCmd(std::move(shellCmd)) {
            this->kind = NodeType::DollarSignNotation;
        }
    };

    std::string StringifyNodeTypes(NodeType type);
}
