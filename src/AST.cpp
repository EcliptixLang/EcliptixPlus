#include <AST.h>
#include <stdexcept>

namespace Ecliptix::AST {
    std::string StringifyNodeTypes(NodeType type) {
        switch (type) {
            case NodeType::Program: return "NodeType::Program";
            case NodeType::BinaryExpression: return "NodeType::BinaryExpression";
            case NodeType::Identifier: return "NodeType::Identifier";
            case NodeType::CallExpression: return "NodeType::CallExpression";
            case NodeType::FunctionDeclaration: return "NodeType::FunctionDeclaration";
            case NodeType::NumericLiteral: return "NodeType::NumericLiteral";
            case NodeType::StringLiteral: return "NodeType::StringLiteral";
            case NodeType::UnaryExpression: return "NodeType::UnaryExpression";
			case NodeType::NullLiteral: return "NodeType::NullLiteral";
            default: throw std::invalid_argument("Unknown node type");
        }
    }
}
