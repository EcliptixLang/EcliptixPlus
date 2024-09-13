#pragma once
#include "Lexer.h"
#include "AST.h"
#include "Generators.h"
#include "Utilities.h"

namespace Ecliptix::Parser {
    class Parser {
    public:
        Ecliptix::AST::Program produceAST(std::string sourceCode);

    private:
        TokenArr Tokens{};
        Ecliptix::Lexer::Token lastToken;

        bool NotEOF();
        Ecliptix::Lexer::Token currentToken();
        Ecliptix::Lexer::Token previousToken();
        Ecliptix::Lexer::Token nextToken();

        Ecliptix::AST::Statement ParseStatement();
        Ecliptix::AST::Expression ParseExpression();
        Ecliptix::AST::Expression ParsePrimaryExpression();
    };
}