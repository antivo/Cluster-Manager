#include "mpi.h"

#include <windows.h>

#include <iostream>
#include <sstream>


BOOL IsProcessRunning(DWORD pid)
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

int main(int argc, char **argv) {
    MPI::Init(argc,argv);
    int i = atoi(argv[1]);
    bool running = IsProcessRunning(i);
    MPI::Finalize();
    return running;
}
