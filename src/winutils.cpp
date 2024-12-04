#include <windows.h>
#include <string>
#include <ENV.hpp>
#include <iostream>
#include <filesystem>
#include <functional>
/*#include <FunctionValues.hpp>
#include <winutils.hpp>

void ModuleLoader::loadModule(const std::string& modulePath, Environment& env) {
        HMODULE hModule = LoadLibraryA(modulePath.c_str());
        if (!hModule) {
            std::cerr << "Error loading module: " << GetLastError() << std::endl;
            return;
        }

        // Get the symbol (function pointer) from the shared object
        typedef IModule* (*createModuleFunc)();
        createModuleFunc createModule = (createModuleFunc)GetProcAddress(hModule, "createModule");

        if (!createModule) {
            std::cerr << "Error loading symbol: " << GetLastError() << std::endl;
            FreeLibrary(hModule);
            return;
        }

        // Create an instance of the module
        IModule* module = createModule();
        module->initialize();

        Module m = module->load();
        for(auto [k, v] : m.Functions){
            env.setVariableSafe(k, std::make_shared<NativeFN>(v), true);
        }

        module->execute();

        // Clean up
        FreeLibrary(hModule);
}*/

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

    /*for(auto& var : eenv.variables){
        if(!enve.variables[var.first]){
            std::cout << "declaring it var " << var.first << "\n";
            enve.variables[var.first] = std::move(var.second);
            enve.constants.push_back(var.first);

            std::cout << enve.variables[var.first]->type() << std::endl;
        } else {
            std::cout << "skipping it var " << var.first << "\n";
        }
    }*/

    return enve;
}

std::vector<std::string> getDLLs(){
    std::vector<std::string> dlls{};
    std::string exeDir = GetExecutableDirectory();
    
    for (const auto& entry : std::filesystem::directory_iterator(exeDir)) {
        if (entry.path().extension() == ".dll") {
            dlls.push_back(exeDir.append("\\").append(entry.path().filename().string()));
        }
    }

    return dlls;
}

int DisplayErrorMessageBox(const char* error) {
    int msgboxID = MessageBoxA(
        NULL,
        error,
        "Error",
        MB_ICONEXCLAMATION | MB_OK
    );

    if (msgboxID == IDOK)
    {
        exit(1);
    }

    return msgboxID;    
}

wchar_t * convertCharArrayToLPCWSTR(const char* charArray){
    wchar_t* wString=new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

void sleep(int idk){
    Sleep(idk);
}