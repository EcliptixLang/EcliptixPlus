#include <windows.h>
#include <string>
#include <ENV.hpp>
#include <iostream>
#include <filesystem>
#include <functional>

std::string GetExecutablePath() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return std::string(path);
}

std::string GetExecutableDirectory() {
    std::string path = GetExecutablePath();
    size_t pos = path.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : path.substr(0, pos);
}
Environment IncludeLIB(std::string lib, Environment &env){
    Environment enve(std::move(env));
    HMODULE hModule = LoadLibraryA(lib.c_str());
    if (!hModule) {
        DWORD errorMessageID = ::GetLastError();
        std::cerr << "Could not load the \"" << lib << "\" Library.\n" << errorMessageID << std::endl;
    }

    typedef Environment (*CreateLibFn)();
    
    CreateLibFn createLibFunction = (CreateLibFn)GetProcAddress(hModule, "createLib");
    std::function<Environment()> myFunction = createLibFunction;

    if (!myFunction) {
        std::cerr << "Could not locate the \"createLib\" function for loading." << std::endl;
        FreeLibrary(hModule);
    }

    std::cout << "running it function\n";
    Environment eenv = myFunction();
    std::cout << "ran it function\n";

    for(auto& var : eenv.variables){
        if(!enve.variables[var.first]){
            std::cout << "declaring it var " << var.first << "\n";
            enve.variables[var.first] = std::move(var.second);
            enve.constants.push_back(var.first);

            std::cout << enve.variables[var.first].get()->type() << std::endl;
        } else {
            std::cout << "skipping it var " << var.first << "\n";
        }
    }

    return enve;
}

std::vector<std::string> getDLLs(){
    std::vector<std::string> dlls{};
    std::string exeDir = GetExecutableDirectory();
    
    for (const auto& entry : std::filesystem::directory_iterator(exeDir)) {
        if (entry.path().extension() == ".dll") {
            //dlls.push_back(entry.path().filename().string());
            dlls.push_back(exeDir.append("\\").append(entry.path().filename().string()));
        }
    }

    return dlls;
}