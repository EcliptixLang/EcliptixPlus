#pragma once
#include "Lexer.hpp"
#include "AST.hpp"
#include "Utilities.hpp"

class Parser {
    public:
        std::shared_ptr<AST::Program> produceAST(std::string& sourceCode);

    private:
        TokenArr Tokens{};
        Lexer::Token lastToken;

		bool NotEOF();
		Lexer::Token currentToken();
        Lexer::Token previousToken();
        Lexer::Token nextToken();

        std::shared_ptr<AST::ExprAST> ParseStatement();
        std::shared_ptr<AST::ExprAST> parseWhen();
        std::shared_ptr<AST::ExprAST> parseWhile();
        std::shared_ptr<AST::ExprAST> parseComments();
        std::shared_ptr<AST::ExprAST> parseIf();
        std::shared_ptr<AST::ExprAST> parseFunctions();
        std::shared_ptr<AST::ExprAST> parseVariables();
        std::shared_ptr<AST::ExprAST> parseAssignment();
        std::shared_ptr<AST::ExprAST> parseArrays();
        std::shared_ptr<AST::ExprAST> parseDSNotation();
        std::shared_ptr<AST::ExprAST> parseObjects();
        std::shared_ptr<AST::ExprAST> parseMemberCalls();
        std::shared_ptr<AST::ExprAST> parseCalls(std::shared_ptr<AST::ExprAST> caller);
        std::vector<std::shared_ptr<AST::ExprAST>> parseArgs();
		std::vector<std::shared_ptr<AST::ExprAST>> parseArgsList();
        std::shared_ptr<AST::ExprAST> parseMember();

        std::shared_ptr<AST::ExprAST> ParseExpression();
        std::shared_ptr<AST::ExprAST> ParseAdditiveExpression();
        std::shared_ptr<AST::ExprAST> ParsePrimary();
        std::shared_ptr<AST::ExprAST> ParseMultiplicativeExpression();
        Lexer::Token expectToken(Lexer::TokenType type);
};