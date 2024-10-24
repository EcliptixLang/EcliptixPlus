#include <ENV.hpp>
#include <Values.hpp>
#include <FunctionValues.hpp>
#include <iostream>
#include <windows.h>

std::unique_ptr<Values::Runtime> hello(std::vector<std::unique_ptr<Values::Runtime>> args, Environment& env){
    Values::Number* num = dynamic_cast<Values::Number*>(std::move(args[0].get()));
    std::cout << num->value << "\n";    
}

extern "C" __declspec(dllexport) Environment createLib(){
    Environment env;
    env.variables["hello"] = std::make_unique<NativeFN>(NativeFN(hello));
    // std::cout << env.lookupVar("hello").get()->type() << "\n";
    return env;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Code to run when the DLL is unloaded
        break;
    }
    return TRUE;
}