#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#define CharArr std::vector<char>

namespace Utilities {
    std::string readFile(std::string name);
    void writeFile(std::string name, std::string value);
    CharArr split(std::string src);

    template<typename T>
    T shift(std::vector<T>& vec) {
        if (vec.empty()) {
            throw std::out_of_range("Cannot shift from an empty vector.");
        }

        T value = vec.front();
        vec.erase(vec.begin());
        return value;
    }
}
