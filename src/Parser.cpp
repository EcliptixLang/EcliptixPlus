#include <Parser.h>
#include <iostream>

namespace Ecliptix::Parser {

    Ecliptix::AST::Program Parser::produceAST(std::string sourceCode) {
        this->Tokens = Ecliptix::Lexer::tokenize(sourceCode);
        Ecliptix::AST::Program program;
        program.kind = Ecliptix::AST::NodeType::Program;

        while (NotEOF()) {
            program.body.push_back(this->ParseStatement());
        }

        return program;
    }

    bool Parser::NotEOF() {
        return this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF;
    }

    Ecliptix::Lexer::Token Parser::currentToken() {
        return this->Tokens[0];
    }

    Ecliptix::Lexer::Token Parser::previousToken() {
        return this->lastToken;
    }

    Ecliptix::Lexer::Token Parser::nextToken() {
        lastToken = this->currentToken();
        return Ecliptix::Utilities::shift(this->Tokens);
    }

    Ecliptix::AST::Statement Parser::ParseStatement() {
        return this->ParseExpression();
    }

    Ecliptix::AST::Expression Parser::ParseExpression() {
        return this->ParsePrimaryExpression();
    }

    Ecliptix::AST::Expression Parser::ParsePrimaryExpression() {
        Ecliptix::Lexer::Token token = this->currentToken();

        switch (token.type) {
            case Ecliptix::Lexer::TokenType::Identifier:
                return Ecliptix::Generators::createIdent(this->nextToken().value);
            case Ecliptix::Lexer::TokenType::Number:
                return Ecliptix::Generators::createNumber(this->nextToken().value);
            default:
                std::cout << "Unexpected token found during parsing" << std::endl;
                exit(1);
        }
    }
}
