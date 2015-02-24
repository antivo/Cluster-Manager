#include "utility_process.h"

#include <windows.h>

#include "assert_common.h"
#include "exception_common.h"

namespace utility {
	unsigned long createProcess(const char* proc, const char* outFile) {
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		HANDLE h = CreateFileA(outFile,
			FILE_APPEND_DATA,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			&sa,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags |= STARTF_USESTDHANDLES;
		si.hStdInput = NULL;
		si.hStdError = h;
		si.hStdOutput = h;
		ZeroMemory(&pi, sizeof(pi));

		auto result = CreateProcessA(NULL, // No module name (use command line).
			//TEXT("proces"), // Command line.
			const_cast<char *>(proc),    // konverzija u LPSTR
			NULL,             // Process handle not inheritable.
			NULL,             // Thread handle not inheritable.
			true,             // Set handle inheritance to FALSE
			0,                // No creation flags.
			NULL,             // Use parent's environment block.
			NULL,             // Use parent's starting directory.
			&si,              // Pointer to STARTUPINFO structure.
			&pi               // Pointer to PROCESS_INFORMATION structure.
		);

		assert::condition(result, []() {return exception::Exception(GetLastError()); });
		DWORD pid = GetProcessId(pi.hProcess);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return pid;
	}
}