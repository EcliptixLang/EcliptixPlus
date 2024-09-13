#include <Lexer.h>
#include <iostream>

namespace Ecliptix::Lexer {

    KeywordList Keywords() {
        KeywordList keywords;
        keywords["set"] = TokenType::Set;
        keywords["lock"] = TokenType::Lock;
        return keywords;
    }

    Token token(std::string value, TokenType type) {
        return { value, type };
    }

    bool skippable(char src) {
        return src == ' ' || src == '\n' || src == '\t' || src == '\b';
    }

    TokenArr tokenize(std::string sourceCode) {
        KeywordList keywords = Keywords();
        TokenArr tokens{};
        CharArr src = Ecliptix::Utilities::split(sourceCode);

        while (src.size() > 0) {
            std::cout << "Current character: " << src[0] << std::endl;

            if (src[0] == '(') {
                tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::OpenParen));
            } else if (src[0] == '=') {
                Ecliptix::Utilities::shift(src);
                tokens.push_back(token("=", TokenType::Equals));
            } else if (src[0] == ')') {
                tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::CloseParen));
            } else if (src[0] == '*' || src[0] == '+' || src[0] == '-' || src[0] == '/' && src[1] != '/') {
                std::string idk = "";
                idk += Ecliptix::Utilities::shift(src);
                tokens.push_back(token(idk, TokenType::BinaryOperator));
            } else {
                if (isalpha(src[0])) {
                    std::string idk = "";
                    while (src.size() > 0 && isalnum(src[0])) {
                        idk += Ecliptix::Utilities::shift(src);
                    }
                    if (keywords.find(idk) == keywords.end()) {
                        tokens.push_back(token(idk, TokenType::Identifier));
                    } else {
                        tokens.push_back(token(idk, keywords[idk]));
                    }
                } else if (isdigit(src[0])) {
                    std::string idk = "";
                    while (src.size() > 0 && isdigit(src[0])) {
                        idk += Ecliptix::Utilities::shift(src);
                    }
                    tokens.push_back(token(idk, TokenType::Number));
                } else if (skippable(src[0])) {
                    Ecliptix::Utilities::shift(src);
                } else {
                    std::cout << "Unrecognized character found: " << src[0] << std::endl;
                    exit(1);
                }
            }
        }

        tokens.push_back(token("", TokenType::_EOF));
        return tokens;
    }

    std::string StringifyTokenTypes(TokenType type) {
        switch (type) {
            case TokenType::BinaryOperator: return "TokenType::BinaryOperator";
            case TokenType::CloseParen: return "TokenType::CloseParen";
            case TokenType::OpenParen: return "TokenType::OpenParen";
            case TokenType::Equals: return "TokenType::Equals";
            case TokenType::Identifier: return "TokenType::Identifier";
            case TokenType::Number: return "TokenType::Number";
            case TokenType::String: return "TokenType::String";
            case TokenType::Lock: return "TokenType::Lock";
            case TokenType::Set: return "TokenType::Set";
            default: return "unknown";
        }
    }
}