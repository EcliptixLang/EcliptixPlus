#include <Parser.hpp>
#include <Interpreter.hpp>
#include <winutils.hpp>
#include <FunctionValues.hpp>

Parser parser;

int runFile(){
	std::string code = "console.out('Welcome to Ecliptix!')set money: number = 0 $'ls'console.out('hi')while(true){$'clear'console.out('Currently, you have money')console.out('[1] Work')console.out('[2] Shop')console.out('[3] Exit')lock thing: string = console.ask('> ')console.out(thing)if(thing == '1'){lock random: number = 60 money = money + random console.out('You worked hard and gained', random)wait(3)} else if(thing == '2'){console.out('This has not been implemented yet')wait(3)} else if(thing == '3'){process.quit(0)}break}";

	std::unique_ptr<AST::ExprAST> ast = parser.produceAST(code);
	
	return 1;
}

int main(int argc, char* argv[]) {
		return runFile();
}