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

        std::unique_ptr<Ecliptix::AST::Statement> ParseStatement();
        std::unique_ptr<Ecliptix::AST::Expression> ParseExpression();
		std::unique_ptr<Ecliptix::AST::Expression> ParseAdditiveExpression();
        std::unique_ptr<Ecliptix::AST::Expression> ParsePrimaryExpression();
		std::unique_ptr<Ecliptix::AST::Expression> ParseMultiplicativeExpression();
		Ecliptix::Lexer::Token expectToken(Ecliptix::Lexer::TokenType);
    };
}