#pragma once
#include <vector>
#include <string>
#include <map>
#include "Utilities.h"

#define TokenArr std::vector<Ecliptix::Lexer::Token>
#define KeywordList std::map<std::string, Ecliptix::Lexer::TokenType>

namespace Ecliptix::Lexer {
    enum class TokenType {
		Null,

        String,
        Number,
        Identifier,

        Equals,
        OpenParen,
        CloseParen,
		OpenBrace,
		CloseBrace,
        BinaryOperator,

        Set,
        Lock,

        _EOF
    };

    struct Token {
        std::string value;
        TokenType type;
    };

    TokenArr tokenize(std::string sourceCode);
    std::string StringifyTokenTypes(TokenType type);
}
