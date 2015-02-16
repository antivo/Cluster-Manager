#include "mpi.h"

#include <windows.h>

#include <string>
#include <sstream>
#include <vector>


DWORD createProcess(const char* proc, const char* outFile) {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;      
    HANDLE h = CreateFile(outFile,
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
   
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = h;
    si.hStdOutput = h;
    ZeroMemory(&pi, sizeof(pi));
   
    if (!CreateProcess(NULL, // No module name (use command line).
        //TEXT("proces"), // Command line.
        const_cast<char *>(proc),    // konverzija u LPSTR
        NULL,             // Process handle not inheritable.
        NULL,             // Thread handle not inheritable.
        true,            // Set handle inheritance to FALSE
        DETACHED_PROCESS,                // No creation flags.
        NULL,             // Use parent's environment block.
        NULL,             // Use parent's starting directory.
        &si,              // Pointer to STARTUPINFO structure.
        &pi)             // Pointer to PROCESS_INFORMATION structure.
        )    
	{ 
        return -1;    
    }
    DWORD pid = GetProcessId(pi.hProcess);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return pid;
}

int main(int argc, char **argv) {
    const int last = argc - 1;
    int pid;
    std::stringstream ss;
   
    MPI::Init(argc,argv);
    bool first = true;
    for(int i = 1; i < last; ++i) {
        if(first) {
            first = false;
        } else {
            ss << " ";
        }
        ss << argv[i] ;
    }
    pid = createProcess(ss.str().c_str(), argv[last]);
    MPI::Finalize();
   
    return pid;
}
