#include <Lexer.h>
#include <iostream>

namespace Ecliptix::Lexer {

    KeywordList Keywords() {
        KeywordList keywords;
        keywords["set"] = TokenType::Set;
        keywords["lock"] = TokenType::Lock;
		keywords["null"] = TokenType::Null;
		keywords["fun"] = TokenType::Fun;
		keywords["if"] = TokenType::If;
		keywords["else"] = TokenType::Else;
		keywords["for"] = TokenType::For;
		keywords["while"] = TokenType::While;
		keywords["return"] = TokenType::Return;
		keywords["break"] = TokenType::Break;
		keywords["take"] = TokenType::Take;
		keywords["from"] = TokenType::From;
		keywords["give"] = TokenType::Give;
		keywords["using"] = TokenType::Using;
		keywords["when"] = TokenType::When;
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
            if (src[0] == '(') {
                tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::OpenParen));
            } else if (src[0] == ')') {
                tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::CloseParen));
            } else if(src[0] == '{') {
				tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::OpenBrace));
			} else if (src[0] == '}') {
                tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::CloseBrace));
            } else if(src[0] == '[') {
				tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::OpenBracket));
			} else if (src[0] == ']') {
                tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::CloseBracket));
            } else if (src[0] == '=') {
                Ecliptix::Utilities::shift(src);
				if(src[1] == '='){
					Ecliptix::Utilities::shift(src);
					tokens.push_back(token("==", TokenType::DoubleEquals));
				} else
                	tokens.push_back(token("=", TokenType::Equals));
            } else if (src[0] == '$') {
                Ecliptix::Utilities::shift(src);
                tokens.push_back(token("$", TokenType::DollarSign));
            } else if (src[0] == ';') {
                Ecliptix::Utilities::shift(src);
                tokens.push_back(token(";", TokenType::Semicolon));
            } else if (src[0] == ':') {
                Ecliptix::Utilities::shift(src);
                tokens.push_back(token(":", TokenType::Colon));
            } else if (src[0] == ',') {
                Ecliptix::Utilities::shift(src);
                tokens.push_back(token(",", TokenType::Comma));
            } else if (src[0] == '.') {
                Ecliptix::Utilities::shift(src);
                tokens.push_back(token(".", TokenType::Dot));
            } else if (src[0] == '\'' || src[0] == '"') {
                char odk = Ecliptix::Utilities::shift(src);
				std::string str = "";
				while(src.size() > 0 && src[0] != odk){
					str += Ecliptix::Utilities::shift(src);
				}

                tokens.push_back(token(str, TokenType::String));
            } else if (src[0] == '*' || src[0] == '+' || src[0] == '-' || src[0] == '/' && src[1] != '/') {
                std::string idk = "";
                idk += Ecliptix::Utilities::shift(src);
                tokens.push_back(token(idk, TokenType::BinaryOperator));
            } else {
                if (isalpha(src[0])) {
                    std::string idk = "";
                    while (src.size() > 0 && isalnum(src[0])) {
                        char text = Ecliptix::Utilities::shift(src);
						idk += text;
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

        tokens.push_back(token("EndOfFile", TokenType::_EOF));
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
			case TokenType::Null: return "TokenType::Null";
            case TokenType::Set: return "TokenType::Set";
			case TokenType::Async: return "TokenType::Async";
			case TokenType::BinaryEquals: return "TokenType::BinaryEquals";
			case TokenType::Break: return "TokenType::Break";
			case TokenType::CloseBrace: return "TokenType::CloseBrace";
			case TokenType::CloseBracket: return "TokenType::CloseBracket";
			case TokenType::Colon: return "TokenType::Colon";
			case TokenType::Comma: return "TokenType::Comma";
			case TokenType::DollarSign: return "TokenType::DollarSign";
			case TokenType::Dot: return "TokenType::Dot";
			case TokenType::DoubleEquals: return "TokenType::DoubleEquals";
			case TokenType::Else: return "TokenType::Else";
			case TokenType::For: return "TokenType::For";
			case TokenType::From: return "TokenType::From";
			case TokenType::Fun: return "TokenType::Fun";
			case TokenType::Give: return "TokenType::Give";
			case TokenType::GreaterThanSign: return "TokenType::GreaterThanSign";
			case TokenType::If: return "TokenType::If";
			case TokenType::NotEquals: return "TokenType::NotEquals";
			case TokenType::OpenBrace: return "TokenType::OpenBrace";
			case TokenType::OpenBracket: return "TokenType::OpenBracket";
			case TokenType::Return: return "TokenType::Return";
			case TokenType::Semicolon: return "TokenType::Semicolon";
			case TokenType::Slash: return "TokenType::Slash";
			case TokenType::Take: return "TokenType::Take";
			case TokenType::Using: return "TokenType::Using";
			case TokenType::When: return "TokenType::When";
			case TokenType::While: return "TokenType::While";
			case TokenType::_EOF: return "TokenType::_EOF";

            default: return "unknown";
        }
    }
}