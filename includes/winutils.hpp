#pragma once
#include <string>
#include <vector>
#include <ENV.hpp>

int DisplayErrorMessageBox(const char* error);
std::string GetExecutableDirectory();
std::vector<std::string> getDLLs();
Environment IncludeLIB(std::string lib, Environment &env);
void sleep(int idk);

#include <ECMOD.hpp>

class ModuleLoader {
public:
    void loadModule(const std::string& modulePath, Environment& env);
};