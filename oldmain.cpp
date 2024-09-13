#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

#define TokenArr std::vector<Ecliptix::Lexer::Token>
#define CharArr std::vector<char>
#define KeywordList std::map<std::string, Ecliptix::Lexer::TokenType>
#define StmtArr std::vector<Statement>

namespace Ecliptix::Utilities {
	std::string readFile(std::string name){
		std::stringstream ss;
		std::fstream file(name, std::ios::in);

		ss << file.rdbuf();

		return ss.str();
	}

	CharArr split(std::string src){
		CharArr st{};
		for (char ch : src) {
			st.push_back(ch);
	    }
		return st;
	}

	template<typename T>
	T shift(std::vector<T>& vec) {
	    if (vec.empty()) {
	        throw std::out_of_range("Cannot shift from an empty vector.");
	    }

	    T value = vec.front();
	    vec.erase(vec.begin());
	    return value;
	}
}

namespace Ecliptix::Lexer {
	enum class TokenType {
		String,
		Number,
		Identifier,
		Equals,
		OpenParen,
		CloseParen,
		BinaryOperator,

		Set,
		Lock,

		_EOF
	};
	struct Token {
		std::string value;
		TokenType type;
	};

	KeywordList Keywords(){
		KeywordList keywords;
		keywords["set"] = TokenType::Set;
		keywords["lock"] = TokenType::Lock;

		return keywords;
	}

	Token token(std::string value, TokenType type){
		return { value, type };
	}

	bool skippable(char src){
		return src == ' ' || src == '\n' || src == '\t' || src == '\b';
	}

	TokenArr tokenize(std::string sourceCode){
		KeywordList keywords = Keywords();
		TokenArr tokens{};
		CharArr src = Ecliptix::Utilities::split(sourceCode);

		while(src.size() > 0){
			std::cout << "Current character: " << src[0] << std::endl;
			if(src[0] == '('){
				tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::OpenParen));
			} else if(src[0] == '='){
				Ecliptix::Utilities::shift(src);
				tokens.push_back(token("=", TokenType::Equals));
			} else if(src[0] == ')'){
				tokens.push_back(token(std::to_string(Ecliptix::Utilities::shift(src)), TokenType::CloseParen));
			} else if(src[0] == '*' || src[0] == '+' || src[0] == '-' || src[0] == '/' && src[1] != '/'){
				std::string idk = "";
				idk += Ecliptix::Utilities::shift(src);
				tokens.push_back(token(idk, TokenType::BinaryOperator));
			} else {
				if(isalpha(src[0])){
					std::string idk = "";
					while(src.size() > 0 && isalnum(src[0])){
						idk += Ecliptix::Utilities::shift(src);
					}
					if(keywords.find(idk) == keywords.end())
						tokens.push_back(token(idk, TokenType::Identifier));
					else {
						tokens.push_back(token(idk, keywords[idk]));
					}
				} else if(isdigit(src[0])){
					std::string idk = "";
					while(src.size() > 0 && isdigit(src[0])){
						idk += Ecliptix::Utilities::shift(src);
					}

					tokens.push_back(token(idk, TokenType::Number));
				} else if(skippable(src[0])){
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

std::string StringifyTokenTypes(TokenType type){
		switch(type){
			case TokenType::BinaryOperator:
				return "TokenType::BinaryOperator";
			case TokenType::CloseParen:
				return "TokenType::CloseParen";
			case TokenType::OpenParen:
				return "TokenType::OpenParen";
			case TokenType::Equals:
				return "TokenType::Equals";
			case TokenType::Identifier:
				return "TokenType::Identifier";
			case TokenType::Number:
				return "TokenType::Number";
			case TokenType::String:
				return "TokenType::String";
			case TokenType::Lock:
				return "TokenType::Lock";
			case TokenType::Set:
				return "TokenType::Set";
			default:
				return "unknown";
		}
	}
}

namespace Ecliptix::AST {
	enum class NodeType {
		Program,
		NumericLiteral,
		StringLiteral,
		Identifier,
		BinaryExpression,
		CallExpression,
		UnaryExpression,
		FunctionDeclaration
	};

	struct Statement {
		NodeType kind;
	};

	struct Program : public Statement {
		StmtArr body;
	};

	struct Expression : public Statement {};

	struct BinaryExpression : public Expression {
		Expression left;
		Expression right;
		std::string _operator;
	};

	struct IdentifierLiteral : public Expression {
		std::string symbol;
	};

	struct NumericLiteral : public Expression {
		int value;
	};

	std::string StringifyNodeTypes(NodeType type){
		switch(type){
			case NodeType::Program:
				return "NodeType::Program";
			case NodeType::BinaryExpression:
				return "NodeType::BinaryExpression";
			case NodeType::Identifier:
				return "NodeType::Identifier";
			case NodeType::CallExpression:
				return "NodeType::CallExpression";
			case NodeType::FunctionDeclaration:
				return "NodeType::FunctionDeclaration";
			case NodeType::NumericLiteral:
				return "NodeType::NumericLiteral";
			case NodeType::StringLiteral:
				return "NodeType::StringLiteral";
			case NodeType::UnaryExpression:
				return "NodeType::UnaryExpression";
			default:
				throw "Unknown type found";
		}
	}
}

namespace Ecliptix::Generators {
	Ecliptix::AST::IdentifierLiteral createIdent(std::string symbol){
		return { Ecliptix::AST::NodeType::Identifier, symbol };
	}
	Ecliptix::AST::NumericLiteral createNumber(std::string num){
		std::cout << num << " and " << std::stoi(num) << std::endl;
		return { Ecliptix::AST::NodeType::NumericLiteral, std::stoi(num) };
	}
}

namespace Ecliptix::Parser {
	class Parser {
		public:
			Ecliptix::AST::Program produceAST(std::string sourceCode){
				this->Tokens = Ecliptix::Lexer::tokenize(sourceCode);
				Ecliptix::AST::Program program;
				program.kind = Ecliptix::AST::NodeType::Program;

				while(NotEOF()){
					program.body.push_back(this->ParseStatement());
				}

				return program;
			}
		private:
			TokenArr Tokens{};
			Ecliptix::Lexer::Token lastToken;
			bool NotEOF(){
				return this->currentToken().type != Ecliptix::Lexer::TokenType::_EOF;
			}
			Ecliptix::Lexer::Token currentToken(){
				return this->Tokens[0];
			}
			Ecliptix::Lexer::Token previousToken(){
				return this->lastToken;
			}
			Ecliptix::Lexer::Token nextToken(){
				lastToken = this->currentToken();
				return Ecliptix::Utilities::shift(this->Tokens);
			}

			Ecliptix::AST::Statement ParseStatement(){
				return this->ParseExpression();
			}
			Ecliptix::AST::Expression ParseExpression(){
				return this->ParsePrimaryExpression();
			}
			Ecliptix::AST::Expression ParsePrimaryExpression(){
				Ecliptix::Lexer::Token token = this->currentToken();

				switch(token.type){
					case Ecliptix::Lexer::TokenType::Identifier:
						return Ecliptix::Generators::createIdent(this->nextToken().value);
					case Ecliptix::Lexer::TokenType::Number:
						return Ecliptix::Generators::createNumber(this->nextToken().value);
					default:
						std::cout << "Unexpected token found during parsing" << std::endl;
						exit(1);
				}
			}
			
	};
}

int main(int argc, char* argv[]) {
    /* if (argc < 2) {
        std::cout << "Not enough arguments\n";
        return 1;
    }
    std::string contents = Ecliptix::Utilities::readFile(argv[1]); */
    
    Ecliptix::Parser::Parser parser;

    std::string code;

    std::cout << "> ";
	std::cin >> code;

    Ecliptix::AST::Program program = parser.produceAST(code);

    std::cout << "{" << std::endl;
    std::cout << "\tkind: " << Ecliptix::AST::StringifyNodeTypes(program.kind) << "," << std::endl;
    std::cout << "\tbody: [" << std::endl;

    for (const Ecliptix::AST::Statement& stmt : program.body) {
        std::cout << "\t\t{" << std::endl;
        std::cout << "\t\t\tkind: " << Ecliptix::AST::StringifyNodeTypes(stmt.kind) << "," << std::endl;

        switch (stmt.kind) {
            case Ecliptix::AST::NodeType::Identifier: {
                Ecliptix::AST::IdentifierLiteral id = static_cast<const Ecliptix::AST::IdentifierLiteral&>(stmt);
                std::cout << "\t\t\tsymbol: \"" << id.symbol << "\"" << std::endl;
                break;
            }
            case Ecliptix::AST::NodeType::NumericLiteral: {
                Ecliptix::AST::NumericLiteral num = static_cast<const Ecliptix::AST::NumericLiteral&>(stmt);
                std::cout << "\t\t\tvalue: " << std::to_string(num.value) << std::endl;
                break;
            }
            default:
                std::cout << "\t\t\t// No additional information for this node type" << std::endl;
                break;
        }
        std::cout << "\t\t}," << std::endl;
    }
    std::cout << "\t]" << std::endl;
    std::cout << "}" << std::endl;

    return 0;
}