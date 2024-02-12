#pragma once
#include "Hyko.h"

#include <functional>

namespace HKCR {
	template<typename T, typename ...Args>
	inline auto loadDLLFunc(const std::string& dllName, const std::string& functionName) -> T(*)(Args...) {
		using Func = T(*)(Args...); // Exportable DLL function pointer

		HINSTANCE hDLL = LoadLibraryA(dllName.c_str());

		if (hDLL) {
			Func resFunctionP = (Func)GetProcAddress(hDLL, functionName.c_str()); // function pointer
			if (resFunctionP == nullptr) {
				printf("DLL function (%s) was not loaded from %s (file: %s, line: %i)\n", functionName.c_str(), dllName.c_str(), __FILE__, __LINE__); // Because the "logger" is also a dll, and it may not be loaded
				return nullptr;
			}
			else
				return resFunctionP;
		}
		
		printf("DLL (%s) was not loaded. Error code: %i (file: %s, line: %i)\n", dllName.c_str(), ERROR_DELAY_LOAD_FAILED, __FILE__, __LINE__); // Because the "logger" is also a dll, and it may not be loaded
		return nullptr;
	}

	void freeDLL(const std::string& dllName) {
		HINSTANCE hDLL = GetModuleHandle(dllName.c_str());
		if (hDLL) {
			FreeLibrary(hDLL);
			return;
		}

		printf("DLL (%s) not founded (file: %s, line: %i)\n", dllName.c_str(), __FILE__, __LINE__); // Because the "logger" is also a dll, and it may not be loaded
	}
}