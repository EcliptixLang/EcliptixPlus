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
			virtual std::unique_ptr<Runtime> clone() const = 0;
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
			
			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<Null>(Null());
      		}
	};

	class Break : public Runtime {
		public:

			Break() {}
			std::string type() const override {
				return "break";
			}
			std::string stringValue() const override {
		        return "<break>";
      		}
			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<Break>(Break());
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

			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<ReturnedValue>(ReturnedValue(value.get()->clone()));
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

			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<Boolean>(Boolean(value));
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

			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<Number>(Number(value));
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

			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<String>(String(value));
      		}
	};

	class ShellCommand : public Runtime {
		public:
			std::string command;
			std::string output;

			ShellCommand(std::string command, std::string output) : command(command), output(output) {}
			std::string type() const override {
				return "shell";
			}

			std::string stringValue() const override {
		        return output;
      		}

			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<ShellCommand>(ShellCommand(command, output));
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

			std::unique_ptr<Runtime> clone() const override {
				std::vector<std::unique_ptr<AST::ExprAST>> newbody;
				for(auto& expr : body){
					newbody.push_back(expr.get()->clone());
				}

		        return std::make_unique<Function>(Function(std::move(newbody), parameters, name, Type));
      		}
	};

	std::string runtimeToJson(const std::unique_ptr<Runtime>& runtime);
	std::string mapToJson(const std::map<std::string, std::unique_ptr<Runtime>>& map);
    std::string arrayToJson(const std::vector<std::unique_ptr<Runtime>>& elements);

	class Object : public Runtime {
		public:
			std::map<std::string, std::unique_ptr<Runtime>> props;

			Object(std::map<std::string, std::unique_ptr<Runtime>> props) : props(std::move(props)) {}
			std::string type() const override {
				return "object";
			}

			std::string stringValue() const override {
		        return mapToJson(props);
      		}
			
			std::unique_ptr<Runtime> clone() const override {
				std::map<std::string, std::unique_ptr<Runtime>> newprops;
				for(auto& [name, val] : props){
					newprops[name] = val.get()->clone();
				}

		        return std::make_unique<Object>(Object(std::move(newprops)));
      		}
	};

	class Array : public Runtime {
		public:
			std::vector<std::unique_ptr<Runtime>> elements;

			Array(std::vector<std::unique_ptr<Runtime>> elements) : elements(std::move(elements)) {}
			std::string type() const override {
				return "array";
			}

			std::string stringValue() const override {
		        return arrayToJson(std::move(elements));
      		}

			std::unique_ptr<Runtime> clone() const override {
				std::vector<std::unique_ptr<Runtime>> newelements;
				for(auto& val : elements){
					newelements.push_back(val.get()->clone());
				}

		        return std::make_unique<Array>(Array(std::move(newelements)));
      		}
	};

	class Skip : public Runtime {
		public:
			Skip() {}
			std::string type() const override {
				return "skip";
			}

			std::string stringValue() const override {
		        return "<skip>";
      		}

			std::unique_ptr<Runtime> clone() const override {
		        return std::make_unique<Skip>(Skip());
      		}
	};

};