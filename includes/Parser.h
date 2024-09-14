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
        std::shared_ptr<Ecliptix::AST::Statement> parseWhen();
        std::shared_ptr<Ecliptix::AST::Statement> parseWhile();
        std::shared_ptr<Ecliptix::AST::Statement> parseComments();
        std::shared_ptr<Ecliptix::AST::Statement> parseIf();
        std::shared_ptr<Ecliptix::AST::Statement> parseFunctions();
        std::shared_ptr<Ecliptix::AST::Statement> parseVariables();
        std::shared_ptr<Ecliptix::AST::Expression> parseAssignment();
        std::shared_ptr<Ecliptix::AST::Expression> parseArrays();
        std::shared_ptr<Ecliptix::AST::Expression> parseDSNotation();
        std::shared_ptr<Ecliptix::AST::Expression> parseObjects();
        std::shared_ptr<Ecliptix::AST::Expression> parseMemberCalls();
        std::shared_ptr<Ecliptix::AST::Expression> parseCalls(std::shared_ptr<Ecliptix::AST::Expression> caller);
        std::vector<std::shared_ptr<Ecliptix::AST::Expression>> parseArgs();
		std::vector<std::shared_ptr<Ecliptix::AST::Expression>> parseArgsList();
        std::shared_ptr<Ecliptix::AST::Expression> parseMember();

        std::shared_ptr<Ecliptix::AST::Expression> ParseExpression();
        std::shared_ptr<Ecliptix::AST::Expression> ParseAdditiveExpression();
        std::shared_ptr<Ecliptix::AST::Expression> ParsePrimaryExpression();
        std::shared_ptr<Ecliptix::AST::Expression> ParseMultiplicativeExpression();
        Ecliptix::Lexer::Token expectToken(Ecliptix::Lexer::TokenType type);
    };
}
