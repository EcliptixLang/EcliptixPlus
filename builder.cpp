#include <iostream>
#include <string>
#include <vector>

void run(std::vector<std::string> arr){
    for(auto& item : arr){
        system(item.c_str());
    }
}

int main(int argc, char* argv[]){
    std::vector<std::string> lib{ 
        "g++ -c src/winutils.cpp -o lib/winutils.o -I./includes",
    	"g++ -c src/Utilities.cpp -o lib/Utilities.o -I./includes",
    	"g++ -c src/Parser.cpp -o lib/Parser.o -I./includes",
        "g++ -c src/Values.cpp -o lib/Values.o -I./includes",
	    "g++ -c src/Lexer.cpp -o lib/Lexer.o -I./includes",
    	"g++ -c src/Interpreter.cpp -o lib/Interpreter.o -I./includes",
	    "g++ -c src/ENV.cpp -o lib/ENV.o -I./includes",
        "g++ -c src/AST.cpp -o lib/AST.o -I./includes",
    	"ar rcs lib/libecx.a lib/ENV.o lib/Values.o lib/AST.o lib/Interpreter.o lib/Lexer.o lib/Parser.o lib/Utilities.o lib/winutils.o",
	    "rm -rf lib/*.o" 
    };
    std::vector<std::string> dll{
        "g++ dll.cpp -shared -o std.dll -L./lib -lecx -I./includes -DBUILDING_DLL"
    };
    std::vector<std::string> exec{
        "g++ src/** -o idk.exe -L. -I./includes -std=c++17 -fno-elide-constructors"
    };

    if(argv[1] == "dll"){
        run(lib);
        run(dll);
    } else if(argv[1] == "lib"){
        run(lib);
    } else if(argv[1] == "all"){
        run(lib);
        run(dll);
        run(exec);
    } else {
        run(exec);
    }
    
}