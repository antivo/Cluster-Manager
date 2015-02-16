#ifndef CONFIGURATION_COMMON
#define CONFIGURATION_COMMON

#include <chrono>

namespace configuration {
	// client choice
	constexpr auto CLIENT_CHOICE_SCHEDULE = "1";
	constexpr auto CLIENT_CHOICE_OBTAIN = "2";
	constexpr auto CLIENT_CHOICE_INFO = "3";
	constexpr auto CLIENT_CHOICE_LOGOUT = "4";

	// clock
	constexpr auto					   TICKS_BEFORE_SLEEP = 5;
	const std::chrono::seconds SLEEP_DURATION     (5);

	// console
	constexpr auto CONSOLE_COLOR = 2;

	// database
	constexpr auto DATABASE_PORT_NUMBER = 4000;
	constexpr auto DATABASE_HOST_NAME = "localhost";
	constexpr auto DATABASE_USER_NAME = "root";
	constexpr auto DATABASE_PASSWORD = "cmadmin";
	constexpr auto DATABASE_NAME = "clusterDB";

	// filesystem
	constexpr auto FILESYSTEM_ROOT_DIRECTORY = R"(\\C11\mpi\_cm)";  //R"(C:\A)"; //R"(\\C11\mpi\_cluster)";
	constexpr auto FILESYSTEM_DATA_DIRECTORY = "data";
	constexpr auto FILESYSTEM_RESULT_DIRECTORY = "recv";

	// master node
	//constexpr auto MASTER_ALIAS = R"(C11)";
	constexpr auto MPIEXEC_PATH = R"(\\C11\mpi\_mpiexec\mpiexec.exe)";
	constexpr auto DETACHED_EXECUTOR_PATH = R"(\\C11\mpi\_executor\exec.exe)";
	constexpr auto ATTACHED_INSPECTOR_PATH = R"(\\C11\mpi\_inspector\inspec.exe)";

	// messages
	constexpr auto SUCCESS_MESSAGE = "OK";
	constexpr auto ERROR_MESSAGE = "ERR";

	// network
	constexpr unsigned short SERVER_PORT = 1200;
	constexpr auto SERVER_ADDRESS = "localhost";

	// serialization
	constexpr auto DEFAULT_DELIMITER = '#';

	// contract
	constexpr auto DEFAULT_SEPARATOR = '|';
}

#endif