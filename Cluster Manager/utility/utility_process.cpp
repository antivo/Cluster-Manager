#include "utility_process.h"

#include <windows.h>

#include "assert_common.h"
#include "exception_common.h"

namespace utility {
	unsigned long createProcessAndWaitForResult(const char * proc) {
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		auto result = CreateProcessA(NULL, // No module name (use command line).
			//TEXT("proces"), // Command line.
			const_cast<char *>(proc),    // konverzija u LPSTR
			NULL,             // Process handle not inheritable.
			NULL,             // Thread handle not inheritable.
			FALSE,            // Set handle inheritance to FALSE
			0,                // No creation flags.
			NULL,             // Use parent's environment block.
			NULL,             // Use parent's starting directory.
			&si,              // Pointer to STARTUPINFO structure.
			&pi);             // Pointer to PROCESS_INFORMATION structure.
		assert::condition(result, []() {return exception::Exception(GetLastError()); });
		
		WaitForSingleObject( pi.hProcess, INFINITE );
		DWORD exitCode;
		result = GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return exitCode;
	}
}