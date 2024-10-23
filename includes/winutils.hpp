#pragma once
#include <string>
#include <vector>
#include <ENV.hpp>

std::string GetExecutableDirectory();
std::vector<std::string> getDLLs();
Environment IncludeLIB(std::string lib, Environment &env);