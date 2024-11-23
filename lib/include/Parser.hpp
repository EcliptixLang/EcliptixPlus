#pragma once
#include "Lexer.hpp"
#include "AST.hpp"
#include "Utilities.hpp"

class Parser {
    public:
        std::unique_ptr<AST::Program> produceAST(std::string& sourceCode);

    private:
        TokenArr Tokens{};
        Lexer::Token lastToken;

		bool NotEOF();
		Lexer::Token currentToken();
        Lexer::Token previousToken();
        Lexer::Token nextToken();

        std::unique_ptr<AST::ExprAST> ParseStatement();
        std::unique_ptr<AST::ExprAST> parseWhen();
        std::unique_ptr<AST::ExprAST> parseWhile();
        std::unique_ptr<AST::ExprAST> parseComments();
        std::unique_ptr<AST::ExprAST> parseIf();
        std::unique_ptr<AST::ExprAST> parseFunctions();
        std::unique_ptr<AST::ExprAST> parseVariables();
        std::unique_ptr<AST::ExprAST> parseAssignment();
        std::unique_ptr<AST::ExprAST> parseArrays();
        std::unique_ptr<AST::ExprAST> parseDSNotation();
        std::unique_ptr<AST::ExprAST> parseObjects();
        std::unique_ptr<AST::ExprAST> parseMemberCalls();
        std::unique_ptr<AST::ExprAST> parseCalls(std::unique_ptr<AST::ExprAST> caller);
        std::vector<std::unique_ptr<AST::ExprAST>> parseArgs();
		std::vector<std::unique_ptr<AST::ExprAST>> parseArgsList();
        std::unique_ptr<AST::ExprAST> parseMember();

        std::unique_ptr<AST::ExprAST> ParseExpression();
        std::unique_ptr<AST::ExprAST> ParseAdditiveExpression();
        std::unique_ptr<AST::ExprAST> ParsePrimary();
        std::unique_ptr<AST::ExprAST> ParseMultiplicativeExpression();
        Lexer::Token expectToken(Lexer::TokenType type);
};