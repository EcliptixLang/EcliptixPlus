#include <Values.hpp>

namespace Values { 
    std::string runtimeToJson(const std::shared_ptr<Runtime> runtime) {
        std::string type = runtime->type();
        if (type == "null") {
            return "null";
        } else if (type == "boolean") {
            return dynamic_cast<Boolean*>(runtime.get())->stringValue();
        } else if (type == "number") {
            return dynamic_cast<Number*>(runtime.get())->stringValue();
        } else if (type == "string") {
            return "\"" + dynamic_cast<String*>(runtime.get())->stringValue() + "\"";
        } else if (type == "object") {
            return mapToJson(dynamic_cast<Object*>(runtime.get())->props);
        } else if (type == "array") {
            return arrayToJson(dynamic_cast<Array*>(runtime.get())->elements);
        } else if (type == "shell") {
            return "\"" + dynamic_cast<ShellCommand*>(runtime.get())->command + "\"";
        } else if (type == "function") {
            return "\"<function>\"";
        }
        return "\"<unknown>\"";
    }

    std::string arrayToJson(const std::vector<std::shared_ptr<Runtime>> elements) {
        std::string json = "[";
        bool first = true;

        for (const auto& element : elements) {
            if (!first) json += ", ";
            json += runtimeToJson(element);
            first = false;
        }

        json += "]";
        return json;
    }

    std::string mapToJson(const std::map<std::string, std::shared_ptr<Runtime>> map) {
        std::string json = "{";
        bool first = true;

        for (const auto& [key, value] : map) {
            if (!first) json += ", ";
            json += "\"" + key + "\": " + runtimeToJson(value);
            first = false;
        }

        json += "}";
        return json;
    }
}