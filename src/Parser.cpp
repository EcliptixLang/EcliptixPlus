#include <Parser.hpp>
#include <AST.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using Token = Lexer::Token;
using TokenType = Lexer::TokenType;
#define PAST std::unique_ptr<AST::ExprAST>

bool Parser::NotEOF() {
  return this->Tokens[0].type != TokenType::_EOF;
}

Token Parser::currentToken() {
  return this->Tokens[0];
}

Token Parser::previousToken() {
  return this->lastToken;
}

Token Parser::nextToken() {
  lastToken = this->currentToken();
  return Utilities::shift(this->Tokens);
}

Token Parser::expectToken(TokenType type){
  Token tok = Utilities::shift(this->Tokens);

	if(Lexer::StringifyTokenTypes(tok.type) != Lexer::StringifyTokenTypes(type)){
		std::cout << "Unexpected token found during parsing\n- Value: " << this->currentToken().value << "\n- Type: " << Lexer::StringifyTokenTypes(this->currentToken().type) << "\n- Expected type: " << Lexer::StringifyTokenTypes(type);
    	exit(1);
	}

	return tok;
}

std::unique_ptr<AST::Program> Parser::produceAST(
	std::string& sourceCode
){
	Tokens = Lexer::tokenize(sourceCode);
	std::vector<PAST> body{};
  
	while(this->NotEOF()){
		body.push_back(this->ParseStatement());
	}

	return std::make_unique<AST::Program>(
		AST::Program(std::move(body))
	);
}

PAST Parser::ParseStatement() {
  switch(this->currentToken().type){
		case TokenType::Set: case TokenType::Lock:
			return this->parseVariables();
		case TokenType::Fun:
			return this->parseFunctions();
		case TokenType::If:
			return this->parseIf();
		case TokenType::When:
			return this->parseWhen();
		case TokenType::While:
			return this->parseWhile();
		case TokenType::DollarSign:
			return this->parseDSNotation();
		default:
			return this->ParseExpression();
	}
}

PAST Parser::parseWhen() {
		this->expectToken(TokenType::When);
		PAST conditional = this->ParseExpression();
		std::vector<PAST> consequent{};
		TokenType _operator = TokenType::BinaryEquals;

		this->expectToken(TokenType::OpenBrace);
		Token token = this->currentToken();

		while(this->NotEOF() && this->Tokens[0].type != TokenType::CloseBrace){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(TokenType::CloseBrace);

		return std::make_unique<AST::WhenDeclaration>(
			AST::WhenDeclaration(
				std::move(conditional), 
				_operator, 
				std::move(consequent)
			)
		);
	}

PAST Parser::parseWhile() {
		this->expectToken(TokenType::While);
		PAST conditional = this->ParseExpression();
		std::vector<PAST> consequent{};
		TokenType _operator = TokenType::BinaryEquals;

		this->expectToken(TokenType::OpenBrace);
		Token token = this->currentToken();

		while(this->NotEOF() && this->Tokens[0].type != TokenType::CloseBrace){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(TokenType::CloseBrace);

		return std::make_unique<AST::WhileDeclaration>(
			AST::WhileDeclaration(
				std::move(conditional), 
				_operator, 
				std::move(consequent)
			)
		);
	}

PAST Parser::parseIf() {
		Token token = this->expectToken(TokenType::If);
		PAST conditional = 
			this->ParseExpression();
		std::vector<PAST> consequent{};
		TokenType _operator = 
			TokenType::BinaryEquals;

		this->expectToken(TokenType::OpenBrace);

		while(
			this->NotEOF() &&
			this->currentToken().type 
				!= TokenType::CloseBrace
		){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(TokenType::CloseBrace);
		
		std::vector<PAST> alternate{};

		if(this->currentToken().type == TokenType::Else){
			this->nextToken();
			if(this->currentToken().type == TokenType::If){
        		alternate.push_back(this->parseIf());
			} else {
				this->expectToken(TokenType::OpenBrace);

				while(
					this->NotEOF() &&
					this->currentToken().type 
						!= TokenType::CloseBrace
				){
					alternate.push_back(this->ParseStatement());
				}

				this->expectToken(TokenType::CloseBrace);
			}
		}

		return std::make_unique<AST::IfStatement>(
			AST::IfStatement(
				std::move(conditional), 
				_operator, 
				std::move(consequent),
				std::move(alternate)
			)
		);
	}

PAST Parser::parseFunctions() {
		Token token = this->nextToken();

		this->expectToken(TokenType::Colon);
		
		std::string type = 
			this->expectToken(TokenType::Identifier).value;

		std::string name = 
			this->expectToken(TokenType::Identifier).value;

		std::vector<PAST> args = 
			this->parseArgs();

		std::vector<std::string> params{};

		for(auto& arg : args){
			AST::ExprAST* expr = arg.get();
			if(expr->getType() != AST::Nodes::Identifier){
				throw "Parameters expected inside function declaration";
			}

	      	AST::Identifier* id = dynamic_cast<AST::Identifier*>(
				arg.get()
			);
			
			params.push_back(id->name);
		}

		this->expectToken(TokenType::OpenBrace);

		std::vector<PAST> body{};

		while(
			this->NotEOF() &&
			this->currentToken().type 
				!= TokenType::CloseBrace
		){
			body.push_back(this->ParseStatement());
		}

		this->expectToken(TokenType::CloseBrace);

		return std::make_unique<AST::Function>(
			AST::Function(
				params, 
				name, 
				std::move(body),
				type
			)
		);
}

PAST Parser::parseVariables() {
		Token token = this->nextToken();
		bool isConstant = token.type == TokenType::Lock;
		std::string ident = 
			this->expectToken(TokenType::Identifier).value;
		std::string type = "";

		this->expectToken(TokenType::Colon);

		type = 
			this->expectToken(TokenType::Identifier).value;

		this->expectToken(TokenType::Equals);

		PAST value = this->ParseStatement();
		return std::make_unique<AST::VariableExpr>(
			AST::VariableExpr(
				ident, 
				type, 
				std::move(value),
				isConstant
			)
		);
	}

PAST Parser::parseAssignment() {
		PAST left = this->parseArrays();

		if(this->currentToken().type == TokenType::Equals){
			Token token = this->nextToken();
			PAST value =
				this->parseAssignment();

			return std::make_unique<AST::AssignmentExpr>(
				AST::AssignmentExpr(
					std::move(left),
					std::move(value)
				)
			);
		}

		return left;
	}

PAST Parser::parseArrays() {
		Token nex = this->currentToken();
		if(nex.type != TokenType::OpenBracket){
			return this->parseDSNotation();
		}

		this->nextToken();

		int num = -1;
		std::vector<PAST> arr{};

		while(this->NotEOF() && this->currentToken().type != TokenType::CloseBracket) {
			PAST key = 
				this->ParseExpression();
			num++;
			TokenType ttype = this->nextToken().type;
			if(
				ttype == TokenType::Comma || 
				ttype == TokenType::CloseBracket
			){
				arr.push_back(std::move(key));
				continue;
			}

			if(ttype != TokenType::CloseBracket){
				if(
					this->currentToken().type == 
					TokenType::Comma
				) throw std::runtime_error("idk");
			}
		}

		this->expectToken(TokenType::CloseBracket);

		return std::make_unique<AST::Array>(
			AST::Array(
				std::move(arr)
			)
		);
	}

PAST Parser::parseDSNotation() {
		if(this->Tokens[0].type != TokenType::DollarSign){
			return this->parseObjects();
		}
		this->nextToken();
		Token idk = this->expectToken(TokenType::String);

		return std::make_unique<AST::ShellCMD>(
			AST::ShellCMD(
				idk.value
			)
		);
	}

PAST Parser::parseObjects() {
		if(this->currentToken().type != TokenType::OpenBrace){
			return this->ParseAdditiveExpression();
		}

		Token token = this->nextToken();
		std::vector<PAST> map;

		while(
			this->NotEOF() && 
			this->currentToken().type 
				!= TokenType::CloseBrace
		){
			std::string key = this->expectToken(TokenType::Identifier).value;

			if (this->currentToken().type == TokenType::Comma) {
				this->nextToken(); 
				map.push_back(
					std::make_unique<AST::Element>(
						AST::Element(
							key, nullptr
						)
					)
				);
				continue;
			} 
			else if (this->currentToken().type == TokenType::CloseBrace) {
				map.push_back(
					std::make_unique<AST::Element>(
						AST::Element(
							key, nullptr
						)
					)
				);
				continue;
			}

			this->expectToken(TokenType::Colon);

			PAST value = 
				this->ParseExpression();
      		map.push_back(
				std::make_unique<AST::Element>(
					AST::Element(
						key, 
						std::move(value)
					)
				)
			);
			if(
				this->currentToken().type != 
				TokenType::CloseBrace
			) this->expectToken(TokenType::Comma);
		}

		this->expectToken(TokenType::CloseBrace);
		return std::make_unique<AST::Object>(
			AST::Object(
				std::move(map)
			)
		);
	}
	
PAST Parser::parseMemberCalls() {
		PAST member = 
			this->parseMember();
		if(
			this->currentToken().type == 
			TokenType::OpenParen
		){
			return this->parseCalls(std::move(member));
		}

		return member;
	}

PAST Parser::parseCalls(PAST caller) {
	AST::NumberExpr* expr = dynamic_cast<AST::NumberExpr*>(
		caller.get()
	);

    if(expr){
      if(expr->getType() == AST::Nodes::Number)
  			this->ParsePrimary();
	}

	std::vector<PAST> args = this->parseArgs();
	PAST call = std::make_unique<AST::CallExpr>(AST::CallExpr(std::move(caller), std::move(args)));
		
	if(this->currentToken().type == TokenType::OpenParen){
		call = this->parseCalls(std::move(call));
	}

	return call;
}

std::vector<PAST> Parser::parseArgs() {
		this->expectToken(TokenType::OpenParen);
		std::vector<PAST> args{};
		if(this->currentToken().type != TokenType::CloseParen) args = this->parseArgsList();
		this->expectToken(TokenType::CloseParen);

		return args;
	}

std::vector<PAST> Parser::parseArgsList() {
		std::vector<PAST> args{};
    	args.push_back(this->parseAssignment());
		while(this->currentToken().type == TokenType::Comma){
			this->nextToken();

			args.push_back(this->parseAssignment());
		}

		return args;
	}

PAST Parser::parseMember() {
		PAST object = this->ParsePrimary();
		while(this->currentToken().type == TokenType::Dot || this->currentToken().type == TokenType::OpenBracket){
			Token _operator = this->nextToken();
			PAST property;
			bool computed;

			if(_operator.type == TokenType::Dot){
				computed = false;
				property = this->ParsePrimary();
				if(property->getType() != AST::Nodes::Identifier){
					throw std::runtime_error("idk");
				}
			} else {
				computed = true;
				property = this->ParseExpression();
				this->expectToken(TokenType::CloseBracket);
			}

			object = std::make_unique<AST::MemberExpr>(AST::MemberExpr(std::move(object), std::move(property), computed));
		}

		return object;
	}


PAST Parser::ParseExpression() {
        return this->parseAssignment();
    }

PAST Parser::ParseAdditiveExpression(){
		PAST left = this->ParseMultiplicativeExpression();
		std::unique_ptr<AST::BinaryExpr> expr;
		while(this->currentToken().value == "+" || this->currentToken().value == "-"){
			std::string _operator = this->nextToken().value;
      PAST right = this->ParseMultiplicativeExpression();

			expr = std::make_unique<AST::BinaryExpr>(AST::BinaryExpr(_operator.c_str()[0], std::move(left), std::move(right)));
			left = std::move(expr);
		}

		return left;
	}

PAST Parser::ParseMultiplicativeExpression(){
		PAST left = this->parseMemberCalls();
		std::unique_ptr<AST::BinaryExpr> expr;
		while(this->currentToken().value == "/" || this->currentToken().value == "*"){
			std::string _operator = this->nextToken().value;
			PAST right = this->parseMemberCalls();
			
			expr = std::make_unique<AST::BinaryExpr>(AST::BinaryExpr(_operator.c_str()[0], std::move(left), std::move(right)));
			left = std::move(expr);
		}

		return left;
	}

PAST Parser::ParsePrimary() {
        Token token = this->currentToken();

        switch (token.type) {
            case TokenType::Identifier:
				return std::make_unique<AST::Identifier>(AST::Identifier(this->nextToken().value));
            case TokenType::Number:
                return std::make_unique<AST::NumberExpr>(AST::NumberExpr(std::stod(this->nextToken().value)));
			case TokenType::Break:
				this->nextToken();
                return std::make_unique<AST::Break>(AST::Break());
			case TokenType::String:
				return std::make_unique<AST::StringExpr>(AST::StringExpr(this->nextToken().value));
			case TokenType::Return:
				this->nextToken();
				return std::make_unique<AST::ReturnExpr>(AST::ReturnExpr(this->ParseExpression()));

			case TokenType::OpenParen:{
				this->nextToken();
				PAST left = this->ParseExpression();
				PAST right;
				PAST value;
				Lexer::Token oper;
				if(this->currentToken().type == TokenType::ComparativeOperator){
					oper = this->nextToken();
					right = this->ParseExpression();
					value = std::make_unique<AST::EquExpr>(AST::EquExpr(std::move(left), std::move(right), oper));
				}else {
					value = std::move(left);
				}
				this->expectToken(TokenType::CloseParen);
				return value;
			}
			case TokenType::NL:
				this->nextToken();
				return ParseStatement();
            default:
                std::cout << "Unexpected token found during parsing\n- Value: " << this->currentToken().value << "\n- Type: " << Lexer::StringifyTokenTypes(this->currentToken().type) << "\n- past: " << Lexer::StringifyTokenTypes(this->lastToken.type) << "\n";
                exit(1);
        }
}