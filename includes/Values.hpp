#pragma once

#include <string>
#include <memory>

namespace Values {
	class Runtime {
		public:
			virtual std::string type() const = 0;
			virtual ~Runtime() = default;
	};

	class Null : public Runtime {
		public:
			std::string value = "null";
			Null() {}
			std::string type() const override {
				return "null";
			}
	};

	class Number : public Runtime {
		public:
			int value;

			Number(int value) : value(value) {}
			std::string type() const override {
				return "number";
			}
	};

	typedef std::unique_ptr<Runtime> __declspec(dllexport) RuntimeVal;
};