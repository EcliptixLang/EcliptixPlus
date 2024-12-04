#pragma once
#include <ENV.hpp>

struct Module {
    std::map<std::string, std::shared_ptr<Values::Runtime>(*)(std::vector<std::shared_ptr<Values::Runtime>>, Environment&)> Functions;
};

class IModule {
public:
    virtual ~IModule() = default;
    virtual void initialize() = 0;
    virtual void execute() = 0;
    virtual Module load() = 0;
};