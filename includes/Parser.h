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

        std::shared_ptr<Ecliptix::AST::Statement> ParseStatement();
        std::shared_ptr<Ecliptix::AST::Expression> ParseExpression();
		std::shared_ptr<Ecliptix::AST::Expression> ParseAdditiveExpression();
        std::shared_ptr<Ecliptix::AST::Expression> ParsePrimaryExpression();
		std::shared_ptr<Ecliptix::AST::Expression> ParseMultiplicativeExpression();
		Ecliptix::Lexer::Token expectToken(Ecliptix::Lexer::TokenType);
    };
}