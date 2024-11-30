#pragma once
#include <string>
#include <vector>

struct ProjSettings {
    std::string name = "Project";
    std::string version = "1.0";
    std::vector<std::string> libs {};
};

struct InterpreterSettings {
    int max_when = 10;
    bool use_new_syntax = false;
};

struct TypeSettings {
    std::string string = "String";
    std::string number = "Number";
    std::string object = "Object";
    std::string array = "Array";
    std::string boolean = "Boolean";
    std::string automatic = "Auto";
    std::string null = "Null";
    std::string shell = "Shell";
};

struct KeywordSettings {
    std::string lock = "lock";
    std::string fun = "fun";
    std::string _if = "if";
    std::string _else = "else";
    std::string _for = "for";
    std::string _while = "while";
    std::string _return = "return";
    std::string set = "set";
    std::string _break = "break";
    std::string take = "take";
    std::string from = "from";
    std::string give = "give";
    std::string _using = "using";
    std::string when = "when";
    std::string skip = "skip";
};

struct Settings {
    ProjSettings project;
    InterpreterSettings interpreter;
    TypeSettings types;
    KeywordSettings keywords;
};

Settings config();