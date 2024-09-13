#include <Utilities.h>
#include <fstream>
#include <sstream>

namespace Ecliptix::Utilities {
    std::string readFile(std::string name) {
        std::stringstream ss;
        std::fstream file(name, std::ios::in);
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
