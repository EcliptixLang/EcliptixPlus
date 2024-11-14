#include <Parser.hpp>
#include <AST.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define Token Lexer::Token

bool Parser::NotEOF() {
  return this->Tokens[0].type != Lexer::TokenType::_EOF;
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

Token Parser::expectToken(Lexer::TokenType type){
  Token tok = Utilities::shift(this->Tokens);

	if(tok.type != type){
		std::cout << "Unexpected token found during parsing\n- Value: " << this->currentToken().value << "\n- Type: " << Lexer::StringifyTokenTypes(this->currentToken().type) << "\n- Expected type: " << Lexer::StringifyTokenTypes(type);
    	exit(1);
	}

	return tok;
}

std::unique_ptr<AST::Program> Parser::produceAST(
	std::string& sourceCode
){
	Tokens = Lexer::tokenize(sourceCode);
	std::vector<std::unique_ptr<AST::ExprAST>> body{};
  
	while(this->NotEOF()){
		body.push_back(this->ParseStatement());
	}

	return std::make_unique<AST::Program>(
		AST::Program(std::move(body))
	);
}

std::unique_ptr<AST::ExprAST> Parser::ParseStatement() {
  switch(this->currentToken().type){
		case Lexer::TokenType::Set: case Lexer::TokenType::Lock:
			return this->parseVariables();
		case Lexer::TokenType::Fun:
			return this->parseFunctions();
		case Lexer::TokenType::If:
			return this->parseIf();
		case Lexer::TokenType::When:
			return this->parseWhen();
		case Lexer::TokenType::While:
			return this->parseWhile();
		case Lexer::TokenType::DollarSign:
			return this->parseDSNotation();
		default:
			return this->ParseExpression();
	}
}

std::unique_ptr<AST::ExprAST> Parser::parseWhen() {
		Token token = this->expectToken(Lexer::TokenType::When);
		std::unique_ptr<AST::ExprAST> conditional = this->ParseExpression();
		std::vector<std::unique_ptr<AST::ExprAST>> consequent{};
		std::unique_ptr<AST::ExprAST> right = {};
		Lexer::TokenType _operator = Lexer::TokenType::BinaryEquals; 

		this->expectToken(Lexer::TokenType::OpenBrace);

		while(this->NotEOF() && 
			this->currentToken().type != 
			Lexer::TokenType::CloseBrace
		){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(Lexer::TokenType::CloseBrace);

		return std::make_unique<AST::WhenDeclaration>(
			AST::WhenDeclaration(
				std::move(conditional), 
				_operator, 
				std::move(consequent)
			)
		);
}

std::unique_ptr<AST::ExprAST> Parser::parseWhile() {
		Token token = this->expectToken(Lexer::TokenType::While);
		std::unique_ptr<AST::ExprAST> conditional = 
			this->ParseExpression();
		std::vector<std::unique_ptr<AST::ExprAST>> consequent{};
		std::unique_ptr<AST::ExprAST> right = {};
		Lexer::TokenType _operator = 
			Lexer::TokenType::BinaryEquals;

		this->expectToken(Lexer::TokenType::OpenBrace);

		while(
			this->NotEOF() && 
			this->currentToken().type !=
				Lexer::TokenType::CloseBrace
		){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(Lexer::TokenType::CloseBrace);

		return std::make_unique<AST::WhileDeclaration>(
			AST::WhileDeclaration(
				std::move(conditional), 
				_operator, 
				std::move(consequent)
			)
		);
	}

std::unique_ptr<AST::ExprAST> Parser::parseIf() {
		Token token = this->expectToken(Lexer::TokenType::If);
		std::unique_ptr<AST::ExprAST> conditional = 
			this->ParseExpression();
		std::vector<std::unique_ptr<AST::ExprAST>> consequent{};
		Lexer::TokenType _operator = 
			Lexer::TokenType::BinaryEquals;

		this->expectToken(Lexer::TokenType::OpenBrace);

		while(
			this->NotEOF() &&
			this->currentToken().type 
				!= Lexer::TokenType::CloseBrace
		){
			consequent.push_back(this->ParseStatement());
		}

		this->expectToken(Lexer::TokenType::CloseBrace);
		
		std::vector<std::unique_ptr<AST::ExprAST>> alternate{};

		if(this->currentToken().type == Lexer::TokenType::Else){
			this->nextToken();
			if(this->currentToken().type == Lexer::TokenType::If){
        		alternate.push_back(this->parseIf());
			} else {
				this->expectToken(Lexer::TokenType::OpenBrace);

				while(
					this->NotEOF() &&
					this->currentToken().type 
						!= Lexer::TokenType::CloseBrace
				){
					alternate.push_back(this->ParseStatement());
				}

				this->expectToken(Lexer::TokenType::CloseBrace);
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

std::unique_ptr<AST::ExprAST> Parser::parseFunctions() {
		Token token = this->nextToken();

		this->expectToken(Lexer::TokenType::Colon);
		
		std::string type = 
			this->expectToken(Lexer::TokenType::Identifier).value;

		std::string name = 
			this->expectToken(Lexer::TokenType::Identifier).value;

		std::vector<std::unique_ptr<AST::ExprAST>> args = 
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

		this->expectToken(Lexer::TokenType::OpenBrace);

		std::vector<std::unique_ptr<AST::ExprAST>> body{};

		while(
			this->NotEOF() &&
			this->currentToken().type 
				!= Lexer::TokenType::CloseBrace
		){
			body.push_back(this->ParseStatement());
		}

		this->expectToken(Lexer::TokenType::CloseBrace);

		return std::make_unique<AST::Function>(
			AST::Function(
				params, 
				name, 
				std::move(body),
				type
			)
		);
}

std::unique_ptr<AST::ExprAST> Parser::parseVariables() {
		Token token = this->nextToken();
		bool isConstant = token.type == Lexer::TokenType::Lock;
		std::string ident = 
			this->expectToken(Lexer::TokenType::Identifier).value;
		std::string type = "";

		this->expectToken(Lexer::TokenType::Colon);

		type = 
			this->expectToken(Lexer::TokenType::Identifier).value;

		this->expectToken(Lexer::TokenType::Equals);

		std::unique_ptr<AST::ExprAST> value = this->ParseStatement();
		return std::make_unique<AST::VariableExpr>(
			AST::VariableExpr(
				ident, 
				type, 
				std::move(value),
				isConstant
			)
		);
	}

std::unique_ptr<AST::ExprAST> Parser::parseAssignment() {
		std::unique_ptr<AST::ExprAST> left = this->parseArrays();

		if(this->currentToken().type == Lexer::TokenType::Equals){
			Token token = this->nextToken();
			std::unique_ptr<AST::ExprAST> value =
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

std::unique_ptr<AST::ExprAST> Parser::parseArrays() {
		Token nex = this->currentToken();
		if(nex.type != Lexer::TokenType::OpenBracket){
			return this->parseDSNotation();
		}

		this->nextToken();

		int num = -1;
		std::vector<std::unique_ptr<AST::ExprAST>> arr{};

		while(this->NotEOF() && this->currentToken().type != Lexer::TokenType::CloseBracket) {
			std::unique_ptr<AST::ExprAST> key = 
				this->ParseExpression();
			num++;
			Lexer::TokenType ttype = this->nextToken().type;
			if(
				ttype == Lexer::TokenType::Comma || 
				ttype == Lexer::TokenType::CloseBracket
			){
				arr.push_back(std::move(key));
				continue;
			}

			if(ttype != Lexer::TokenType::CloseBracket){
				if(
					this->currentToken().type == 
					Lexer::TokenType::Comma
				) throw std::runtime_error("idk");
			}
		}

		this->expectToken(Lexer::TokenType::CloseBracket);

		return std::make_unique<AST::Array>(
			AST::Array(
				std::move(arr)
			)
		);
	}

std::unique_ptr<AST::ExprAST> Parser::parseDSNotation() {
		if(this->Tokens[0].type != Lexer::TokenType::DollarSign){
			return this->parseObjects();
		}
		this->nextToken();
		Token idk = this->expectToken(Lexer::TokenType::String);

		return std::make_unique<AST::ShellCMD>(
			AST::ShellCMD(
				idk.value
			)
		);
	}

std::unique_ptr<AST::ExprAST> Parser::parseObjects() {
		if(this->currentToken().type != Lexer::TokenType::OpenBrace){
			return this->ParseAdditiveExpression();
		}

		Token token = this->nextToken();
		std::vector<std::unique_ptr<AST::ExprAST>> map;

		while(
			this->NotEOF() && 
			this->currentToken().type 
				!= Lexer::TokenType::CloseBrace
		){
			std::string key = this->expectToken(Lexer::TokenType::Identifier).value;

			if (this->currentToken().type == Lexer::TokenType::Comma) {
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
			else if (this->currentToken().type == Lexer::TokenType::CloseBrace) {
				map.push_back(
					std::make_unique<AST::Element>(
						AST::Element(
							key, nullptr
						)
					)
				);
				continue;
			}

			this->expectToken(Lexer::TokenType::Colon);

			std::unique_ptr<AST::ExprAST> value = 
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
				Lexer::TokenType::CloseBrace
			) this->expectToken(Lexer::TokenType::Comma);
		}

		this->expectToken(Lexer::TokenType::CloseBrace);
		return std::make_unique<AST::Object>(
			AST::Object(
				std::move(map)
			)
		);
	}
	
std::unique_ptr<AST::ExprAST> Parser::parseMemberCalls() {
		std::unique_ptr<AST::ExprAST> member = 
			this->parseMember();
		if(
			this->currentToken().type == 
			Lexer::TokenType::OpenParen
		){
			return this->parseCalls(std::move(member));
		}

		return member;
	}

std::unique_ptr<AST::ExprAST> Parser::parseCalls(std::unique_ptr<AST::ExprAST> caller) {
	AST::NumberExpr* expr = dynamic_cast<AST::NumberExpr*>(
		caller.get()
	);

    if(expr){
      if(expr->getType() == AST::Nodes::Number)
  			this->ParsePrimary();
	}

	std::vector<std::unique_ptr<AST::ExprAST>> args = this->parseArgs();
	std::unique_ptr<AST::ExprAST> call = std::make_unique<AST::CallExpr>(AST::CallExpr(std::move(caller), std::move(args)));
		
	if(this->currentToken().type == Lexer::TokenType::OpenParen){
		call = this->parseCalls(std::move(call));
	}

	return call;
}

std::vector<std::unique_ptr<AST::ExprAST>> Parser::parseArgs() {
		this->expectToken(Lexer::TokenType::OpenParen);
		std::vector<std::unique_ptr<AST::ExprAST>> args{};
		if(this->currentToken().type != Lexer::TokenType::CloseParen) args = this->parseArgsList();
		this->expectToken(Lexer::TokenType::CloseParen);

		return args;
	}

std::vector<std::unique_ptr<AST::ExprAST>> Parser::parseArgsList() {
		std::vector<std::unique_ptr<AST::ExprAST>> args{};
    	args.push_back(this->parseAssignment());
		while(this->currentToken().type == Lexer::TokenType::Comma){
			this->nextToken();

			args.push_back(this->parseAssignment());
		}

		return args;
	}

std::unique_ptr<AST::ExprAST> Parser::parseMember() {
		std::unique_ptr<AST::ExprAST> object = this->ParsePrimary();
		while(this->currentToken().type == Lexer::TokenType::Dot || this->currentToken().type == Lexer::TokenType::OpenBracket){
			Token _operator = this->nextToken();
			std::unique_ptr<AST::ExprAST> property;
			bool computed;

			if(_operator.type == Lexer::TokenType::Dot){
				computed = false;
				property = this->ParsePrimary();
				if(property.get()->getType() != AST::Nodes::Identifier){
					throw std::runtime_error("idk");
				}
			} else {
				computed = true;
				property = this->ParseExpression();
				this->expectToken(Lexer::TokenType::CloseBracket);
			}

			object = std::make_unique<AST::MemberExpr>(AST::MemberExpr(std::move(object), std::move(property), computed));
		}

		return object;
	}


std::unique_ptr<AST::ExprAST> Parser::ParseExpression() {
        return this->parseAssignment();
    }

std::unique_ptr<AST::ExprAST> Parser::ParseAdditiveExpression(){
		std::unique_ptr<AST::ExprAST> left = this->ParseMultiplicativeExpression();
		std::unique_ptr<AST::BinaryExpr> expr;
		while(this->currentToken().value == "+" || this->currentToken().value == "-"){
			std::string _operator = this->nextToken().value;
      std::unique_ptr<AST::ExprAST> right = this->ParseMultiplicativeExpression();

			expr = std::make_unique<AST::BinaryExpr>(AST::BinaryExpr(_operator.c_str()[0], std::move(left), std::move(right)));
			left = std::move(expr);
		}

		return left;
	}

std::unique_ptr<AST::ExprAST> Parser::ParseMultiplicativeExpression(){
		std::unique_ptr<AST::ExprAST> left = this->parseMemberCalls();
		std::unique_ptr<AST::BinaryExpr> expr;
		while(this->currentToken().value == "/" || this->currentToken().value == "*"){
			std::string _operator = this->nextToken().value;
			std::unique_ptr<AST::ExprAST> right = this->parseMemberCalls();
			
			expr = std::make_unique<AST::BinaryExpr>(AST::BinaryExpr(_operator.c_str()[0], std::move(left), std::move(right)));
			left = std::move(expr);
		}

		return left;
	}

std::unique_ptr<AST::ExprAST> Parser::ParsePrimary() {
        Token token = this->currentToken();

        switch (token.type) {
            case Lexer::TokenType::Identifier:
				return std::make_unique<AST::Identifier>(AST::Identifier(this->nextToken().value));
            case Lexer::TokenType::Number:
                return std::make_unique<AST::NumberExpr>(AST::NumberExpr(std::stod(this->nextToken().value)));
			case Lexer::TokenType::Break:
				this->nextToken();
                return std::make_unique<AST::Break>(AST::Break());
			case Lexer::TokenType::String:
				return std::make_unique<AST::StringExpr>(AST::StringExpr(this->nextToken().value));
			case Lexer::TokenType::Return:
				this->nextToken();
				return std::make_unique<AST::ReturnExpr>(AST::ReturnExpr(this->ParseExpression()));

			case Lexer::TokenType::OpenParen:{
				this->nextToken();
				std::unique_ptr<AST::ExprAST> left = this->ParseExpression();
				std::unique_ptr<AST::ExprAST> right;
				std::unique_ptr<AST::ExprAST> value;
				Lexer::TokenType oper;
				if(this->currentToken().type == Lexer::TokenType::DoubleEquals || this->currentToken().type == Lexer::TokenType::NotEquals){
					oper = this->nextToken().type;
					right = this->ParseExpression();
					value = std::make_unique<AST::EquExpr>(AST::EquExpr(std::move(left), std::move(right), oper));
				}else {
					value = std::move(left);
				}
				this->expectToken(Lexer::TokenType::CloseParen);
				return value;
			}
			case Lexer::TokenType::NL:
				this->nextToken();
				return ParseStatement();
            default:
                std::cout << "Unexpected token found during parsing\n- Value: " << this->currentToken().value << "\n- Type: " << Lexer::StringifyTokenTypes(this->currentToken().type) << "\n- past: " << Lexer::StringifyTokenTypes(this->lastToken.type) << "\n";
                exit(1);
        }
}