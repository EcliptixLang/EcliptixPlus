#pragma once
#include <string>

namespace Values {
	enum class ValueType {
		Null,
		Number
	};

	class Runtime {
		ValueType type;

		public:
			virtual ~Runtime() = default;
	};

	class Null : public Runtime {
		std::string value = "null";

		public:
			Null() {}
	};

	class Number : public Runtime {
		int value;

		public:
			Number(int value) : value(value) {}
	};

};