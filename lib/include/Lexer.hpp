#pragma once
#include <vector>
#include <string>
#include <map>
#include "Utilities.hpp"

#define TokenArr std::vector<Lexer::Token>
#define KeywordList std::map<std::string, Lexer::TokenType>

namespace Lexer {
    enum class TokenType {
		Number,
		Identifier,
		String,
		BinaryOperator,
		Equals,
		BinaryEquals,
		ComparativeOperator,
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
		Slash,
		Set,
		Lock,
		Fun,
		If,
		Skip,
		Else,
		While,
		For,
		Return,
		Break,
		Async,
		From,
		NL,
		Take,
		Give,
		At,
		Using,
		When,
		_EOF,
    };

    struct Token {
        std::string value;
        TokenType type;
    };

    TokenArr tokenize(std::string sourceCode);
    std::string StringifyTokenTypes(TokenType type);
}
