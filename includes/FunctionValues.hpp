#pragma once
#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <Values.hpp>
#include <ENV.hpp>

class DLL_API NativeFN : public Values::Runtime {
	public:
		std::unique_ptr<Values::Runtime>(*call)(std::vector<std::unique_ptr<Values::Runtime>>, Environment&);
        
		NativeFN(std::unique_ptr<Values::Runtime>(*call)(std::vector<std::unique_ptr<Values::Runtime>>, Environment&)) : call(std::move(call)) {}

		std::string type() const override {
			return "native-fn";
		}
	};