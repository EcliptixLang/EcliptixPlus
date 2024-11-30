#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "tomlcpp.hpp"
#include <config.hpp>
#include <map>

using std::cerr;
using std::cout;

void error(std::string msg)
{
    cerr << "ERROR: " << msg << "\n";
    exit(1);
}
void warn(std::string msg)
{
    cerr << "WARNING: " << msg << "\n";
}

Settings config() {
    Settings settings;
    toml::Result res = toml::parseFile("config.ecc");
    if (!res.table) {
        warn("cannot parse config file, using defaults...");
        return settings;
    }

    std::unique_ptr<toml::Table> proj = res.table->getTable("project");
    if (!proj) {
        warn("missing [project] table");
    } else {

        std::pair<bool, std::string> projname = proj->getString("name");
        if (!projname.first) {
            warn("missing or bad project name entry");
        } else {
            settings.project.name = projname.second;
        }

        std::pair<bool, std::string> projver = proj->getString("version");
        if (!projver.first) {
            warn("missing or bad project version entry");
        } else {
            settings.project.version = projver.second;
        }

        auto libs = proj->getArray("libs");
        if (!libs) {
            warn("missing or bad project library list entry");
        } else {
            std::vector<std::string> strs{};
            for (int i = 0; ; i++) {
                auto p = libs->getString(i);
                if (!p.first) break;

                strs.push_back(p.second);
            }
            settings.project.libs = strs;
        }
    }

    std::unique_ptr<toml::Table> interpreter = res.table->getTable("interpreter");
    if (!interpreter) {
        warn("missing [interpreter] table");
    } else {

        std::pair<bool, int64_t> wennie = interpreter->getInt("max_when");
        if (!wennie.first) {
            warn("missing or bad interpreter max when entry");
        } else {
            settings.interpreter.max_when = wennie.second;
        }

        std::pair<bool, bool> suntax = interpreter->getBool("use_new_syntax");
        if (!suntax.first) {
            warn("missing or bad interpreter new syntax entry");
        } else {
            settings.interpreter.use_new_syntax = suntax.second;
        }
    }

    std::unique_ptr<toml::Table> types = res.table->getTable("types");
    if (!types) {
        warn("missing [types] table");
    } else {
        std::pair<bool, std::string> oto = types->getString("auto");
        if (!oto.first) {
            warn("missing or bad auto type entry");
        } else {
            settings.types.automatic = oto.second;
        }

        std::pair<bool, std::string> nol = types->getString("null");
        if (!nol.first) {
            warn("missing or bad null type entry");
        } else {
            settings.types.null = nol.second;
        }

        std::pair<bool, std::string> shell = types->getString("shell");
        if (!shell.first) {
            warn("missing or bad shell type entry");
        } else {
            settings.types.shell = shell.second;
        }

        std::pair<bool, std::string> strong = types->getString("string");
        if (!strong.first) {
            warn("missing or bad string type entry");
        } else {
            settings.types.string = strong.second;
        }
        std::pair<bool, std::string> numbi = types->getString("number");
        if (!numbi.first) {
            warn("missing or bad number type entry");
        } else {
            settings.types.number = numbi.second;
        }

        std::pair<bool, std::string> obji = types->getString("object");
        if (!obji.first) {
            warn("missing or bad object type entry");
        } else {
            settings.types.object = obji.second;
        }
        std::pair<bool, std::string> arai = types->getString("array");
        if (!arai.first) {
            warn("missing or bad array type entry");
        } else {
            settings.types.array = arai.second;
        }

        std::pair<bool, std::string> bolia = types->getString("boolean");
        if (!bolia.first) {
            warn("missing or bad boolean type entry");
        } else {
            settings.types.boolean = bolia.second;
        }
    }

    std::unique_ptr<toml::Table> keywords = res.table->getTable("keywords");
    if (!proj) {
        warn("missing [keywords] table");
    } else {
        std::vector<std::string> kws { "lock", "fun", "if", "else", "for", "while", "return", "set", "break", "take", "from", 
            "give", "using", "when", "skip"
        };
        std::vector<std::string> cvt {};
        for(auto kw : kws){
            std::pair<bool, std::string> kword = keywords->getString(kw);
            if (!kword.first) {
                warn("missing or bad keyword entry: " + kw);
                cvt.push_back(kw);
            } else {
                cvt.push_back(kword.second);
            }
        }

        settings.keywords.lock = cvt[0]; settings.keywords.fun = cvt[1]; settings.keywords._if = cvt[2];
        settings.keywords._else = cvt[3]; settings.keywords._for = cvt[4]; settings.keywords._while = cvt[5];
        settings.keywords._return = cvt[6]; settings.keywords.set = cvt[7]; settings.keywords._break = cvt[8];
        settings.keywords.take = cvt[9]; settings.keywords.from = cvt[10]; settings.keywords.give = cvt[11];
        settings.keywords._using = cvt[12]; settings.keywords.when = cvt[13]; settings.keywords.skip = cvt[14];
    }
    
    return settings;
}