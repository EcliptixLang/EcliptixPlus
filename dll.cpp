#include <iostream>
#include <windows.h>
#include "ECMOD.hpp"

std::shared_ptr<Values::Runtime> thingy(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    std::cout << "Hello World!";
    return std::make_shared<Values::Null>();
}

class MyModule : public IModule {
public:
    void initialize() override {
        std::cout << "MyModule initialized!" << std::endl;
    }

    Module load() override {
        Module mod;
        mod.Functions["hello"] = thingy;
        return mod;
    }

    void execute() override {
        std::cout << "MyModule executing... " << std::endl;
    }
};

extern "C" __declspec(dllexport) IModule* createModule() {
    std::cout << "HI\n";
    return new MyModule();
}