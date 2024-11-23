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
    	"g++ -c src/evaluation/array.cpp -o lib/array.o -I./includes",
        "g++ -c src/evaluation/assignment.cpp -o lib/assignment.o -I./includes",
        "g++ -c src/evaluation/binary.cpp -o lib/binary.o -I./includes",
        "g++ -c src/evaluation/call.cpp -o lib/call.o -I./includes",
        "g++ -c src/evaluation/equality.cpp -o lib/equality.o -I./includes",
        "g++ -c src/evaluation/eval.cpp -o lib/eval.o -I./includes",
        "g++ -c src/evaluation/function.cpp -o lib/function.o -I./includes",
        "g++ -c src/evaluation/ident.cpp -o lib/ident.o -I./includes",
        "g++ -c src/evaluation/if.cpp -o lib/if.o -I./includes",
        "g++ -c src/evaluation/member.cpp -o lib/member.o -I./includes",
        "g++ -c src/evaluation/number.cpp -o lib/number.o -I./includes",
        "g++ -c src/evaluation/object.cpp -o lib/object.o -I./includes",
        "g++ -c src/evaluation/program.cpp -o lib/program.o -I./includes",
        "g++ -c src/evaluation/string.cpp -o lib/string.o -I./includes",
        "g++ -c src/evaluation/variable.cpp -o lib/variable.o -I./includes",
        "g++ -c src/evaluation/while.cpp -o lib/while.o -I./includes",
	    "g++ -c src/ENV.cpp -o lib/ENV.o -I./includes",
        "g++ -c src/AST.cpp -o lib/AST.o -I./includes",
    	"ar rcs lib/libecx.a lib/ENV.o lib/Values.o lib/AST.o lib/array.o lib/assignment.o lib/binary.o lib/call.o lib/equality.o lib/eval.o lib/function.o lib/ident.o lib/if.o lib/member.o lib/number.o lib/object.o lib/program.o lib/string.o lib/variable.o lib/while.o lib/Lexer.o lib/Parser.o lib/Utilities.o lib/winutils.o",
	    "rm -rf lib/*.o" 
    };
    std::vector<std::string> dll{
        "g++ dll.cpp -shared -o std.dll -L./lib -lecx -I./includes -DBUILDING_DLL"
    };
    std::vector<std::string> exec{
        "g++ src/*.cpp src/evaluation/*.cpp -o idk.exe -L. -I./includes -std=c++17 -fno-elide-constructors"
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