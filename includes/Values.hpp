#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <AST.hpp>

namespace Values {
	class Runtime {
		public:
			virtual std::string type() const = 0;
			virtual ~Runtime() = default;
			virtual std::string stringValue() const = 0;
	};

	class Null : public Runtime {
		public:
			std::string value = "null";
			Null() {}
			std::string type() const override {
				return "null";
			}
			std::string stringValue() const override {
		        return "null";
      		}
	};

	class ReturnedValue : public Runtime {
		public:
			std::unique_ptr<Runtime> value;
			ReturnedValue(std::unique_ptr<Runtime> value) : value(std::move(value)) {}
			std::string type() const override {
				return "return";
			}

			std::string stringValue() const override {
		        return "returnedValue";
      		}
	};
	
	class Boolean : public Runtime {
		public:
			bool value;
			Boolean(bool value) : value(value) {}
			std::string type() const override {
				return "boolean";
			}

			std::string stringValue() const override {
		        return std::to_string(value);
      		}
	};

	class Number : public Runtime {
		public:
			int value;

			Number(int value) : value(value) {}
			std::string type() const override {
				return "number";
			}
			std::string stringValue() const override {
		        return std::to_string(value);
      		}
	};

	class String : public Runtime {
		public:
			std::string value;

			String(std::string value) : value(value) {}
			std::string type() const override {
				return "string";
			}

			std::string stringValue() const override {
		        return value;
      		}
	};

	class ShellCommand : public Runtime {
		public:
			std::string command;

			ShellCommand(std::string command) : command(command) {}
			std::string type() const override {
				return "shell";
			}

			std::string stringValue() const override {
		        return command;
      		}
	};

	class Function : public Runtime {
		public:
			std::vector<std::unique_ptr<AST::ExprAST>> body;
			std::vector<std::string> parameters;
			std::string name;
			std::string Type;

			Function(
				std::vector<std::unique_ptr<AST::ExprAST>> body,
				std::vector<std::string> parameters,
				std::string name,
				std::string Type
			) : body(std::move(body)), parameters(parameters), name(name), Type(Type) {}
			
			std::string type() const override {
				return "function";
			}

			std::string stringValue() const override {
		        return "null";
      		}
	};

	std::string runtimeToJson(const std::unique_ptr<Runtime>& runtime);
	std::string mapToJson(const std::map<std::string, std::unique_ptr<Runtime>>& map);
    std::string arrayToJson(const std::vector<std::unique_ptr<Runtime>>& elements);

	class Object : public Runtime {
		public:
			std::map<std::string, std::unique_ptr<Runtime>> props;

			Object(std::map<std::string, std::unique_ptr<Runtime>>& props) : props(std::move(props)) {}
			std::string type() const override {
				return "object";
			}

			std::string stringValue() const override {
		        return mapToJson(props);
      		}
			
	};

	class Array : public Runtime {
		public:
			std::vector<std::unique_ptr<Runtime>> elements;

			Array(std::vector<std::unique_ptr<Runtime>>& elements) : elements(std::move(elements)) {}
			std::string type() const override {
				return "array";
			}

			std::string stringValue() const override {
		        return arrayToJson(std::move(elements));
      		}
	};

};