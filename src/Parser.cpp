#include <Parser.h>
#include <iostream>
#include <nlohmann/json.hpp>

#define ExpressionPointer std::unique_ptr<Ecliptix::AST::Expression>
#define Token Ecliptix::Lexer::Token


namespace Ecliptix::Parser {

    Ecliptix::AST::Program Parser::produceAST(std::string sourceCode) {
        this->Tokens = Ecliptix::Lexer::tokenize(sourceCode);
        Ecliptix::AST::Program program(Ecliptix::AST::NodeType::Program, std::vector<std::unique_ptr<Ecliptix::AST::Statement>>{});

        program.kind = Ecliptix::AST::NodeType::Program;

        while (NotEOF()) {
            program.body.push_back(this->ParseStatement());
        }
		
        return program;
    }

    bool Parser::NotEOF() {
        return this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF;
    }

    Token Parser::currentToken() {
        return this->Tokens[0];
    }

    Token Parser::previousToken() {
        return this->lastToken;
    }

    Token Parser::nextToken() {
        lastToken = this->currentToken();
        return Ecliptix::Utilities::shift(this->Tokens);
    }

	Token Parser::expectToken(Ecliptix::Lexer::TokenType type){
		Token tok = Ecliptix::Utilities::shift(this->Tokens);

		if(tok.type == type){
			return tok;
		}

		throw "Unexpected token found while parsing";
	}

    std::unique_ptr<Ecliptix::AST::Statement> Parser::ParseStatement() {
        return this->ParseExpression();
    }

    ExpressionPointer Parser::ParseExpression() {
        return this->ParseAdditiveExpression();
    }

	ExpressionPointer Parser::ParseAdditiveExpression(){
		ExpressionPointer left = this->ParseMultiplicativeExpression();
		std::unique_ptr<Ecliptix::AST::BinaryExpression> expr = std::make_unique<Ecliptix::AST::BinaryExpression>();
		while(this->currentToken().value == "+" || this->currentToken().value == "-"){
			std::string _operator = this->nextToken().value;
			ExpressionPointer right = this->ParsePrimaryExpression();

			expr->left = std::move(left);
			expr->right = std::move(right);
			expr->kind = Ecliptix::AST::NodeType::BinaryExpression;
			expr->_operator = _operator;

			left = std::move(expr);
	        expr = std::make_unique<Ecliptix::AST::BinaryExpression>();
		}

		return left;
	}

	ExpressionPointer Parser::ParseMultiplicativeExpression(){
		ExpressionPointer left = this->ParsePrimaryExpression();
		std::unique_ptr<Ecliptix::AST::BinaryExpression> expr = std::make_unique<Ecliptix::AST::BinaryExpression>();
		while(this->currentToken().value == "/" || this->currentToken().value == "*"){
			std::string _operator = this->nextToken().value;
			ExpressionPointer right = this->ParsePrimaryExpression();

			expr->left = std::move(left);
			expr->right = std::move(right);
			expr->kind = Ecliptix::AST::NodeType::BinaryExpression;
			expr->_operator = _operator;

			left = std::move(expr);
        	expr = std::make_unique<Ecliptix::AST::BinaryExpression>();
		}

		return left;
	}

    ExpressionPointer Parser::ParsePrimaryExpression() {
        Token token = this->currentToken();

        switch (token.type) {
            case Ecliptix::Lexer::TokenType::Identifier:
                return Ecliptix::Generators::createIdent(this->nextToken().value);
            case Ecliptix::Lexer::TokenType::Number:
                return Ecliptix::Generators::createNumber(this->nextToken().value);
			case Ecliptix::Lexer::TokenType::OpenParen:{
				this->nextToken();
				ExpressionPointer value = this->ParseExpression();
				this->expectToken(Ecliptix::Lexer::TokenType::CloseParen);
				return value;
			}
			case Ecliptix::Lexer::TokenType::Null:
				this->nextToken();
				return Ecliptix::Generators::createNull();
            default:
                std::cout << "Unexpected token found during parsing" << std::endl;
                exit(1);
        }
    }
}
