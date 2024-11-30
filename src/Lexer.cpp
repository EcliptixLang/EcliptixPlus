#include <Lexer.hpp>
#include <iostream>
#include <config.hpp>

std::string trst(std::string str, Settings settings){
    if(settings.types.automatic == str){
        return "Auto";
    }

    if(settings.types.array == str){
        return "Array";
    }

    if(settings.types.boolean == str){
        return "Boolean";
    }

    if(settings.types.null == str){
        return "Null";
    }

    if(settings.types.number == str){
        return "Number";
    }

    if(settings.types.object == str){
        return "Object";
    }

    if(settings.types.shell == str){
        return "Shell";
    }

    if(settings.types.string == str){
        return "String";
    }

    return str;
}

namespace Lexer {

    KeywordList Keywords(Settings settings) {
        KeywordList keywords;
        if (settings.interpreter.use_new_syntax) {
            keywords[settings.types.automatic] = TokenType::Type;
            keywords[settings.types.boolean] = TokenType::Type;
            keywords[settings.types.array] = TokenType::Type;
            keywords[settings.types.number] = TokenType::Type;
            keywords[settings.types.object] = TokenType::Type;
            keywords[settings.types.string] = TokenType::Type;
            keywords[settings.types.null] = TokenType::Type;
            keywords[settings.types.shell] = TokenType::Type;
        }

        keywords[settings.keywords.set] = TokenType::Set;
        keywords[settings.keywords.lock] = TokenType::Lock;
		keywords[settings.keywords.fun] = TokenType::Fun;
		keywords[settings.keywords._if] = TokenType::If;
		keywords[settings.keywords._else] = TokenType::Else;
		keywords[settings.keywords._for] = TokenType::For;
		keywords[settings.keywords._while] = TokenType::While;
		keywords[settings.keywords._return] = TokenType::Return;
		keywords[settings.keywords._break] = TokenType::Break;
		keywords[settings.keywords.take] = TokenType::Take;
		keywords[settings.keywords.from] = TokenType::From;
		keywords[settings.keywords.give] = TokenType::Give;
		keywords[settings.keywords._using] = TokenType::Using;
		keywords[settings.keywords.when] = TokenType::When;
        keywords[settings.keywords.skip] = TokenType::Skip;
        return keywords;
    }

    Token token(std::string value, TokenType type) {
        return { value, type };
    }

    bool skippable(char src) {
        return src == ' ' || src == '\n' || src == '\t' || src == '\b';
    }

    TokenArr tokenize(std::string sourceCode, Settings settings) {
        KeywordList keywords = Keywords(settings);
        TokenArr tokens{};
        CharArr src = Utilities::split(sourceCode);

        while (src.size() > 0) {
            if (src[0] == '(') {
                tokens.push_back(token(std::to_string(Utilities::shift(src)), TokenType::OpenParen));
            } else if (src[0] == ')') {
                tokens.push_back(token(std::to_string(Utilities::shift(src)), TokenType::CloseParen));
            } else if(src[0] == '{') {
				tokens.push_back(token(std::to_string(Utilities::shift(src)), TokenType::OpenBrace));
			} else if (src[0] == '}') {
                tokens.push_back(token(std::to_string(Utilities::shift(src)), TokenType::CloseBrace));
            } else if(src[0] == '[') {
				tokens.push_back(token(std::to_string(Utilities::shift(src)), TokenType::OpenBracket));
			} else if (src[0] == ']') {
                tokens.push_back(token(std::to_string(Utilities::shift(src)), TokenType::CloseBracket));
            } else if (src[0] == '=') {
                Utilities::shift(src);
				if(src[0] == '='){
					Utilities::shift(src);
					tokens.push_back(token("==", TokenType::ComparativeOperator));
				} else
                	tokens.push_back(token("=", TokenType::Equals));
            } else if (src[0] == '$') {
                Utilities::shift(src);
                tokens.push_back(token("$", TokenType::DollarSign));
            } else if (src[0] == ';') {
                Utilities::shift(src);
                tokens.push_back(token(";", TokenType::Semicolon));
            } else if (src[0] == '@') {
                Utilities::shift(src);
                if(src[1] == '='){
                    Utilities::shift(src);
                    tokens.push_back(token("@=", TokenType::ComparativeOperator));
                } else {
                    tokens.push_back(token("@", TokenType::At));
                }
            } else if (src[0] == '<') {
                Utilities::shift(src);
                if(src[1] == '='){
                    Utilities::shift(src);
                    tokens.push_back(token("<=", TokenType::ComparativeOperator));
                } else {
                    tokens.push_back(token("<", TokenType::ComparativeOperator));
                }
            } else if (src[0] == '>') {
                Utilities::shift(src);
                if(src[1] == '='){
                    Utilities::shift(src);
                    tokens.push_back(token(">=", TokenType::ComparativeOperator));
                } else {
                    tokens.push_back(token(">", TokenType::ComparativeOperator));
                }
            } else if (src[0] == ':') {
                Utilities::shift(src);
                tokens.push_back(token(":", TokenType::Colon));
            } else if (src[0] == ',') {
                Utilities::shift(src);
                tokens.push_back(token(",", TokenType::Comma));
            } else if (src[0] == '.') {
                Utilities::shift(src);
                tokens.push_back(token(".", TokenType::Dot));
            } else if (src[0] == '\'' || src[0] == '"') {
                char odk = Utilities::shift(src);
				std::string str = "";
				while(src.size() > 0 && src[0] != odk){
					str += Utilities::shift(src);
				}
                Utilities::shift(src);

                tokens.push_back(token(str, TokenType::String));
            } else if (src[0] == '*' || src[0] == '+' || src[0] == '-' || src[0] == '/') {
                if (src[1] == '/' || src[1] == '*') {
                    char commentType = src[1];
                    Utilities::shift(src);
                    Utilities::shift(src);

                    if (commentType == '/'){
                        while (!src.empty() && src[0] != '\n') {
                            Utilities::shift(src);
                        }
                    }

                    if (commentType == '*'){
                        Utilities::shift(src);
                        while (!src.empty()) {
                            if (src[0] == '*' && src.size() > 1 && src[1] == '/') {
                                Utilities::shift(src);
                                Utilities::shift(src);
                                break;
                            }
                            Utilities::shift(src);
                        }
                    }
                } else {
                    std::string idk = "";
                    idk += Utilities::shift(src);
                    tokens.push_back(token(idk, TokenType::BinaryOperator));
                }
            } else {
                if (isalpha(src[0])) {
                    std::string idk = "";
                    while (src.size() > 0 && isalnum(src[0])) {
                        char text = Utilities::shift(src);
						idk += text;
                    }
                    if (keywords.find(idk) == keywords.end()) {
                        tokens.push_back(token(idk, TokenType::Identifier));
                    } else {
                        tokens.push_back(token(trst(idk, settings), keywords[idk]));
                    }
                } else if (isdigit(src[0])) {
                    std::string idk = "";
                    while (src.size() > 0 && isdigit(src[0])) {
                        idk += Utilities::shift(src);
                    }
                    tokens.push_back(token(idk, TokenType::Number));
                } else if (skippable(src[0])) {
                    Utilities::shift(src);
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
            case TokenType::BinaryOperator:  return "TokenType::BinaryOperator";
            case TokenType::CloseParen:      return "TokenType::CloseParen";
            case TokenType::OpenParen:       return "TokenType::OpenParen";
            case TokenType::Equals:          return "TokenType::Equals";
            case TokenType::Identifier:      return "TokenType::Identifier";
            case TokenType::Number:          return "TokenType::Number";
            case TokenType::String:          return "TokenType::String";
            case TokenType::Lock:            return "TokenType::Lock";
            case TokenType::Set:             return "TokenType::Set";
			case TokenType::Async:           return "TokenType::Async";
			case TokenType::BinaryEquals:    return "TokenType::BinaryEquals";
			case TokenType::Break:           return "TokenType::Break";
			case TokenType::CloseBrace:      return "TokenType::CloseBrace";
			case TokenType::CloseBracket:    return "TokenType::CloseBracket";
			case TokenType::Colon:           return "TokenType::Colon";
			case TokenType::Comma:           return "TokenType::Comma";
			case TokenType::DollarSign:      return "TokenType::DollarSign";
			case TokenType::Dot:             return "TokenType::Dot";
			case TokenType::ComparativeOperator:    return "TokenType::ComparativeOperator";
			case TokenType::Else:            return "TokenType::Else";
			case TokenType::For:             return "TokenType::For";
			case TokenType::From:            return "TokenType::From";
			case TokenType::Fun:             return "TokenType::Fun";
			case TokenType::Give:            return "TokenType::Give";
			case TokenType::If:              return "TokenType::If";
			case TokenType::OpenBrace:       return "TokenType::OpenBrace";
			case TokenType::OpenBracket:     return "TokenType::OpenBracket";
			case TokenType::Return:          return "TokenType::Return";
			case TokenType::Semicolon:       return "TokenType::Semicolon";
			case TokenType::Slash:           return "TokenType::Slash";
			case TokenType::Take:            return "TokenType::Take";
			case TokenType::Using:           return "TokenType::Using";
			case TokenType::When:            return "TokenType::When";
			case TokenType::While:           return "TokenType::While";
			case TokenType::_EOF:            return "TokenType::_EOF";

            default:                         return "unknown";
        }
    }
}