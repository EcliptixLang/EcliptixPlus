#pragma once

#include <Values.hpp>
#include <ENV.hpp>

class NativeFN : public Values::Runtime {
	public:
		std::unique_ptr<Values::Runtime>(*call)(std::vector<std::unique_ptr<Values::Runtime>>, Environment&);
        
		NativeFN(std::unique_ptr<Values::Runtime>(*call)(std::vector<std::unique_ptr<Values::Runtime>>, Environment&)) : call(std::move(call)) {}

		std::string type() const override {
			return "native-fn";
		}

		std::string stringValue() const override {
			return "null";
		}

		std::unique_ptr<Values::Runtime> clone() const override {
          return std::make_unique<NativeFN>(NativeFN(call));
        }
};

class ProtoFN : public Values::Runtime {
	public:
		std::unique_ptr<Values::Runtime>(*call)(std::vector<std::unique_ptr<Values::Runtime>>, Environment&, std::unique_ptr<Values::Runtime>&);
        
		ProtoFN(std::unique_ptr<Values::Runtime>(*call)(std::vector<std::unique_ptr<Values::Runtime>>, Environment&, std::unique_ptr<Values::Runtime>&)) : call(std::move(call)) {}

		std::string type() const override {
			return "proto-fn";
		}

		std::string stringValue() const override {
			return "null";
		}
};