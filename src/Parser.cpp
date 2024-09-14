#include <Parser.h>
#include <iostream>
#include <nlohmann/json.hpp>

#define StatementPointer std::shared_ptr<Ecliptix::AST::Statement>
#define ExpressionPointer std::shared_ptr<Ecliptix::AST::Expression>
#define ExprArr std::vector<std::shared_ptr<Ecliptix::AST::Expression>>
#define Token Ecliptix::Lexer::Token


namespace Ecliptix::Parser {

    Ecliptix::AST::Program Parser::produceAST(std::string sourceCode) {
        this->Tokens = Ecliptix::Lexer::tokenize(sourceCode);
        Ecliptix::AST::Program program(Ecliptix::AST::NodeType::Program, std::vector<std::shared_ptr<Ecliptix::AST::Statement>>{});

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

    StatementPointer Parser::ParseStatement() {
        switch(this->currentToken().type){
			case Ecliptix::Lexer::TokenType::Slash:
				return this->parseComments();
			case Ecliptix::Lexer::TokenType::Set:
			case Ecliptix::Lexer::TokenType::Lock:
				return this->parseVariables();
			case Ecliptix::Lexer::TokenType::Fun:
				return this->parseFunctions();
			case Ecliptix::Lexer::TokenType::If:
				return this->parseIf();
			case Ecliptix::Lexer::TokenType::When:
				return this->parseWhen();
			case Ecliptix::Lexer::TokenType::While:
				return this->parseWhile();
			case Ecliptix::Lexer::TokenType::DollarSign:
				return this->parseDSNotation();
			default:
				return this->ParseExpression();
		}
    }

	StatementPointer Parser::parseWhen() {
		Token token = this->expectToken(Ecliptix::Lexer::TokenType::When);
		ExpressionPointer conditional = this->ParseExpression();
		StmtArr consequent{};
		ExpressionPointer right = {};
		Ecliptix::Lexer::TokenType _operator = Ecliptix::Lexer::TokenType::BinaryEquals; 

		this->expectToken(Ecliptix::Lexer::TokenType::OpenBrace);

		while(this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(Ecliptix::Lexer::TokenType::CloseBrace);

		return Ecliptix::Generators::createWhenDeclaration(conditional, _operator, consequent);
	}

	StatementPointer Parser::parseWhile() {
		Token token = this->expectToken(Ecliptix::Lexer::TokenType::While);
		ExpressionPointer conditional = this->ParseExpression();
		StmtArr consequent{};
		ExpressionPointer right = {};
		Ecliptix::Lexer::TokenType _operator = Ecliptix::Lexer::TokenType::BinaryEquals;

		this->expectToken(Ecliptix::Lexer::TokenType::OpenBrace);

		while(this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(Ecliptix::Lexer::TokenType::CloseBrace);

		return Ecliptix::Generators::createWhileStatement(conditional, _operator, consequent);
	}

	StatementPointer Parser::parseComments() {
		this->nextToken();

		while(this->nextToken().type != Ecliptix::Lexer::TokenType::Slash && this->NotEOF()){
			continue;
		}

		return this->ParsePrimaryExpression();
	}

	StatementPointer Parser::parseIf() {
		Token token = this->expectToken(Ecliptix::Lexer::TokenType::If);
		ExpressionPointer conditional = this->ParseExpression();
		StmtArr consequent{};
		Ecliptix::Lexer::TokenType _operator = Ecliptix::Lexer::TokenType::BinaryEquals;

		this->expectToken(Ecliptix::Lexer::TokenType::OpenBrace);

		while(this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(Ecliptix::Lexer::TokenType::CloseBrace);
		
		StmtArr alternate{};

		if(this->currentToken().type == Ecliptix::Lexer::TokenType::Else){
			this->nextToken();
			if(this->currentToken().type == Ecliptix::Lexer::TokenType::If){
				alternate = {this->parseIf()};
			} else {
				alternate = {};

				this->expectToken(Ecliptix::Lexer::TokenType::OpenBrace);

				while(this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace){
					alternate.push_back(this->ParseStatement());
				}

				this->expectToken(Ecliptix::Lexer::TokenType::CloseBrace);
			}
		}

		return Ecliptix::Generators::createIfStatement(conditional, _operator, consequent, alternate);
	}

	StatementPointer Parser::parseFunctions() {
		Token token = this->nextToken();
		std::string name = this->expectToken(Ecliptix::Lexer::TokenType::Identifier).value;
		ExprArr args = this->parseArgs();

		std::vector<std::string> params{};

		for(ExpressionPointer arg : args){
			Ecliptix::AST::Expression* expr = arg.get();
			if(expr->kind != Ecliptix::AST::NodeType::Identifier){
				throw "Parameters expected inside function declaration";
			}

			Ecliptix::AST::IdentifierLiteral* id = dynamic_cast<Ecliptix::AST::IdentifierLiteral*>(arg.get());
			params.push_back(id->symbol);
		}

		this->expectToken(Ecliptix::Lexer::TokenType::OpenBrace);

		StmtArr body{};

		while(this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace){
			body.push_back(this->ParseStatement());
		}

		this->expectToken(Ecliptix::Lexer::TokenType::CloseBrace);

		return Ecliptix::Generators::createFunctionDeclaration(params, name, body, Ecliptix::AST::NodeType::FunctionDeclaration);
	}

	StatementPointer Parser::parseVariables() {
		Token token = this->nextToken();
		bool isConstant = token.type == Ecliptix::Lexer::TokenType::Lock;
		std::string ident = this->expectToken(Ecliptix::Lexer::TokenType::Identifier).value;
		std::string type = "";

		this->nextToken();

		type = this->expectToken(Ecliptix::Lexer::TokenType::Identifier).value;

		this->expectToken(Ecliptix::Lexer::TokenType::Equals);

		ExpressionPointer value = this->ParseExpression();
		return Ecliptix::Generators::createVarDeclaration(isConstant, ident, value);
	}

	ExpressionPointer Parser::parseAssignment() {
		ExpressionPointer left = this->parseArrays();

		if(this->currentToken().type == Ecliptix::Lexer::TokenType::Equals){
			Token token = this->nextToken();
			ExpressionPointer value = this->parseAssignment();

			return Ecliptix::Generators::createAssignmentExpr(left, value);
		}

		return left;
	}

	ExpressionPointer Parser::parseArrays() {
		Token nex = this->currentToken();
		if(nex.type != Ecliptix::Lexer::TokenType::OpenBracket){
			return this->parseDSNotation();
		}

		this->nextToken();

		int num = -1;
		std::vector<std::shared_ptr<Ecliptix::AST::ArrayElement>> arr{};

		while(this->NotEOF() && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBracket) {
			ExpressionPointer key = this->ParseExpression();
			num++;
			Ecliptix::Lexer::TokenType ttype = this->nextToken().type;
			if(ttype == Ecliptix::Lexer::TokenType::Comma || ttype == Ecliptix::Lexer::TokenType::CloseBracket){
				arr.push_back(Ecliptix::Generators::createArrayElement(num, key));
				continue;
			}

			if(ttype != Ecliptix::Lexer::TokenType::CloseBracket){
				if(this->currentToken().type == Ecliptix::Lexer::TokenType::Comma) throw std::runtime_error("idk");
			}

			this->expectToken(Ecliptix::Lexer::TokenType::CloseBracket);

			return Ecliptix::Generators::createArrayLiteral(arr);
		}
	}

	ExpressionPointer Parser::parseDSNotation() {
		Token token = this->currentToken();
		if(this->Tokens[1].type != Ecliptix::Lexer::TokenType::DollarSign){
			return this->parseObjects();
		}
		this->nextToken();
		Token idk = this->expectToken(Ecliptix::Lexer::TokenType::String);

		return Ecliptix::Generators::createDollarSignNotation(Ecliptix::Generators::createString(idk.value));
	}

	ExpressionPointer Parser::parseObjects() {
		if(this->currentToken().type != Ecliptix::Lexer::TokenType::OpenBrace){
			return this->ParseAdditiveExpression();
		}

		Token token = this->nextToken();
		std::vector<std::shared_ptr<Ecliptix::AST::Property>> properties{};

		while(this->NotEOF() && this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace){
			std::string key = this->expectToken(Ecliptix::Lexer::TokenType::Identifier).value;

			this->expectToken(Ecliptix::Lexer::TokenType::Colon);

			ExpressionPointer value = this->ParseExpression();
			properties.push_back(Ecliptix::Generators::createProperty(key, value));
			if(this->currentToken().type != Ecliptix::Lexer::TokenType::CloseBrace)
				this->expectToken(Ecliptix::Lexer::TokenType::Comma);
		}

		this->expectToken(Ecliptix::Lexer::TokenType::CloseBrace);
		return Ecliptix::Generators::createObjectLiteral(properties);
	}
	
	ExpressionPointer Parser::parseMemberCalls() {
		ExpressionPointer member = this->parseMember();
		if(this->currentToken().type == Ecliptix::Lexer::TokenType::OpenParen){
			return this->parseCalls(member);
		}

		return member;
	}

	ExpressionPointer Parser::parseCalls(ExpressionPointer caller) {
		Ecliptix::AST::Expression expr =* caller.get();
		if(expr.kind == Ecliptix::AST::NodeType::NumericLiteral){
			this->ParsePrimaryExpression();
		}

		ExpressionPointer call = Ecliptix::Generators::createCallExpr(this->parseArgs(), caller);
		if(this->currentToken().type == Ecliptix::Lexer::TokenType::OpenParen){
			call = this->parseCalls(call);
		}

		return call;
	}
	ExprArr Parser::parseArgs() {
		this->expectToken(Ecliptix::Lexer::TokenType::OpenParen);
		ExprArr args;
		if(this->currentToken().type == Ecliptix::Lexer::TokenType::CloseParen) args = {}; else args = this->parseArgsList();
		this->expectToken(Ecliptix::Lexer::TokenType::CloseParen);

		return args;
	}

	ExprArr Parser::parseArgsList() {
		ExprArr args{this->parseAssignment()};
		while(this->currentToken().type == Ecliptix::Lexer::TokenType::Comma){
			this->nextToken();
			args.push_back(this->parseAssignment());
		}

		return args;
	}

	ExpressionPointer Parser::parseMember() {
		ExpressionPointer object = this->ParsePrimaryExpression();
		while(this->currentToken().type == Ecliptix::Lexer::TokenType::Dot || this->currentToken().type == Ecliptix::Lexer::TokenType::OpenBracket){
			Token _operator = this->nextToken();
			ExpressionPointer property;
			bool computed;

			if(_operator.type == Ecliptix::Lexer::TokenType::Dot){
				computed = false;
				property = this->ParsePrimaryExpression();
				if(property.get()->kind != Ecliptix::AST::NodeType::Identifier){
					throw std::runtime_error("idk");
				}
			} else {
				computed = true;
				property = this->ParseExpression();
				this->expectToken(Ecliptix::Lexer::TokenType::CloseBracket);
			}

			object = Ecliptix::Generators::createMemberExpr(object, property, computed);
		}

		return object;
	}


    ExpressionPointer Parser::ParseExpression() {
        return this->parseAssignment();
    }

	ExpressionPointer Parser::ParseAdditiveExpression(){
		ExpressionPointer left = this->ParseMultiplicativeExpression();
		std::shared_ptr<Ecliptix::AST::BinaryExpression> expr = std::make_shared<Ecliptix::AST::BinaryExpression>();
		while(this->currentToken().value == "+" || this->currentToken().value == "-"){
			std::string _operator = this->nextToken().value;
			ExpressionPointer right = this->ParseMultiplicativeExpression();

			expr->left = std::move(left);
			expr->right = std::move(right);
			expr->kind = Ecliptix::AST::NodeType::BinaryExpression;
			expr->_operator = _operator;

			left = std::move(expr);
	        expr = std::make_shared<Ecliptix::AST::BinaryExpression>();
		}

		return left;
	}

	ExpressionPointer Parser::ParseMultiplicativeExpression(){
		ExpressionPointer left = this->ParsePrimaryExpression();
		std::shared_ptr<Ecliptix::AST::BinaryExpression> expr = std::make_shared<Ecliptix::AST::BinaryExpression>();
		while(this->currentToken().value == "/" || this->currentToken().value == "*"){
			std::string _operator = this->nextToken().value;
			ExpressionPointer right = this->ParsePrimaryExpression();

			expr->left = std::move(left);
			expr->right = std::move(right);
			expr->kind = Ecliptix::AST::NodeType::BinaryExpression;
			expr->_operator = _operator;

			left = std::move(expr);
        	expr = std::make_shared<Ecliptix::AST::BinaryExpression>();
		}

		return left;
	}

    ExpressionPointer Parser::ParsePrimaryExpression() {
        Token token = this->currentToken();

        switch (token.type) {
            case Ecliptix::Lexer::TokenType::Identifier:
                return Ecliptix::Generators::createIdent(this->nextToken().value);
            case Ecliptix::Lexer::TokenType::Number:
                return Ecliptix::Generators::createNumber(std::stoi(this->nextToken().value));
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
                std::cout << "Unexpected token found during parsing" << this->currentToken().value << std::endl;
                exit(1);
        }
    }
}
