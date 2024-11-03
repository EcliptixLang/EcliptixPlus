#include <AST.hpp>

namespace AST {
    std::string stringifyAST(Nodes type){
        switch(type){
            case Nodes::Number: return "number";
            case Nodes::Array: return "array";
            case Nodes::Assignment: return "assignment";
            case Nodes::Binary: return "binary";
            case Nodes::Call: return "call";
            case Nodes::Element: return "element";
            case Nodes::Equality: return "equality";
            case Nodes::Function: return "function";
            case Nodes::Identifier: return "identifier";
            case Nodes::If: return "if";
            case Nodes::Member: return "member";
            case Nodes::Object: return "object";
            case Nodes::Program: return "program";
            case Nodes::ShellCmd: return "shell";
            case Nodes::String: return "string";
            case Nodes::Variable: return "variable";
            case Nodes::When: return "when";
            case Nodes::While: return "while";
            default: return "unknown";
        }
    }
}