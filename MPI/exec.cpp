#include "mpi.h"

#include <windows.h>

#include <string>
#include <sstream>

DWORD createProcess(const char* proc) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    // Start the child process.
    if (!CreateProcess(NULL, // No module name (use command line).
            //TEXT("proces"), // Command line.
            const_cast<char *>(proc),    // konverzija u LPSTR
            NULL,             // Process handle not inheritable.
            NULL,             // Thread handle not inheritable.
            FALSE,            // Set handle inheritance to FALSE
            DETACHED_PROCESS,                // No creation flags.
            NULL,             // Use parent's environment block.
            NULL,             // Use parent's starting directory.
            &si,              // Pointer to STARTUPINFO structure.
            &pi)             // Pointer to PROCESS_INFORMATION structure.
            )     {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }
    DWORD pid = GetProcessId(pi.hProcess);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return pid;
}

int main(int argc, char **argv) {
    MPI::Init(argc,argv);
    std::stringstream ss;
    bool first = true;
    for(int i = 1; i < argc; ++i) {
        if(first) {
            first = false;
        } else {
            ss << " ";
        }
        ss << argv[i] ;
    }
    DWORD process = createProcess(order.c_str());
    MPI::Finalize();
    return process;
}
