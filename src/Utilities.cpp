#include <Utilities.hpp>
#include <fstream>
#include <sstream>

namespace Utilities {
    std::string readFile(std::string name) {
       std::ifstream file(name);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + name);
        }

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    CharArr split(std::string src) {
        CharArr st{};
        for (char ch : src) {
            st.push_back(ch);
        }
        return st;
    }
}