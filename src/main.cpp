#include <Parser.hpp>
#include <Interpreter.hpp>
#include <winutils.hpp>
#include <FunctionValues.hpp>

Parser parser;
Environment ParentENV;

int runFile(std::string filePath){
	std::string code = Utilities::readFile(filePath);

	std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
	Interpreter::evaluate(ast, ParentENV);
	
	return 1;
}

int runREPL(){
    while(true){
        std::cout << "> ";
        std::string code;
        std::getline(std::cin, code);
        if(code == "exit")
            exit(0);

        std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
        Interpreter::evaluate(ast, ParentENV);
    }

    return 0;
}


int main(int argc, char* argv[]) {
    
	if(argc > 1)
		return runFile(argv[1]);
	else
		return runREPL();
}