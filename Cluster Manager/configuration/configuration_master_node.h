#ifndef CONFIGURATION_MASTER_NODE
#define CONFIGURATION_MASTER_NODE

namespace configuration {
	constexpr auto masterAlias = R"(C11)";
	constexpr auto mpiexecPath = R"(C:\Program Files\MPICH2\bin\mpiexec.exe)";
	constexpr auto resultFilePath = R"(\\C11\mpi\_result\out.txt)";
	constexpr auto detachedExecutorPath = R"(\\C11\mpi\_executor\exec.exe)";
	constexpr auto attachedInspectorPath = R"(\\C11\mpi\_inspector\inspec.exe)";
}

#endif