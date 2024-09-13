#pragma once
#include <string>

namespace Ecliptix::Values {
	enum class ValueType {
		Null,
		Number
	};

	struct RuntimeValue {
		ValueType type;

		virtual ~RuntimeValue() = default;
	};

	struct NullValue : public RuntimeValue {
    	std::string value;

    	NullValue() : value("null") {
	        this->type = ValueType::Null;
    	}
	};


	struct NumberValue : public RuntimeValue {
    int value;

    NumberValue(int value) : value(value) {
        this->type = ValueType::Number;
    }
};


} // namespace Ecliptix::Values
