#include <Parser.h>
#include <iostream>

int main() {
    Ecliptix::Parser::Parser parser;

    std::string code;

    std::cout << "> ";
    std::cin >> code;

    Ecliptix::AST::Program program = parser.produceAST(code);

    std::cout << "{" << std::endl;
    std::cout << "\tkind: " << Ecliptix::AST::StringifyNodeTypes(program.kind) << "," << std::endl;
    std::cout << "\tbody: [" << std::endl;

    for (const Ecliptix::AST::Statement& stmt : program.body) {
        std::cout << "\t\t{" << std::endl;
        std::cout << "\t\t\tkind: " << Ecliptix::AST::StringifyNodeTypes(stmt.kind) << "," << std::endl;

        switch (stmt.kind) {
            case Ecliptix::AST::NodeType::Identifier: {
                Ecliptix::AST::IdentifierLiteral id = static_cast<const Ecliptix::AST::IdentifierLiteral&>(stmt);
                std::cout << "\t\t\tsymbol: \"" << id.symbol << "\"" << std::endl;
                break;
            }
            case Ecliptix::AST::NodeType::NumericLiteral: {
                Ecliptix::AST::NumericLiteral num = static_cast<const Ecliptix::AST::NumericLiteral&>(stmt);
                std::cout << "\t\t\tvalue: " << std::to_string(num.value) << std::endl;
                break;
            }
            default:
                std::cout << "\t\t\t// No additional information for this node type" << std::endl;
                break;
        }
        std::cout << "\t\t}," << std::endl;
    }
    std::cout << "\t]" << std::endl;
    std::cout << "}" << std::endl;

    return 0;
}