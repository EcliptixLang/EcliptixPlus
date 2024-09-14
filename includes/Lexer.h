#pragma once
#include <vector>
#include <string>
#include <map>
#include "Utilities.h"

#define TokenArr std::vector<Ecliptix::Lexer::Token>
#define KeywordList std::map<std::string, Ecliptix::Lexer::TokenType>

namespace Ecliptix::Lexer {
    enum class TokenType {
		Number,
		Identifier,
		String,
		BinaryOperator,
		Equals,
		BinaryEquals,
		DoubleEquals,
		NotEquals,
		Comma,
		DollarSign,
		Dot,
		Colon,
		Semicolon,
		OpenParen, 
		CloseParen, 
		OpenBrace, 
		CloseBrace,
	    OpenBracket, 
		CloseBracket,
		GreaterThanSign,
		Slash,
		Set,
		Lock,
		Fun,
		If,
		Else,
		While,
		For,
		Return,
		Break,
		Async,
		From,
		Take,
		Give,
		Using,
		When,
		Null,
		_EOF,
    };

    struct Token {
        std::string value;
        TokenType type;
    };

    TokenArr tokenize(std::string sourceCode);
    std::string StringifyTokenTypes(TokenType type);
}
