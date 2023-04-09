#include "Processes.hpp"

bool	ExecuteFile::Handle()
{
	int	pipe_fd = executor.executeToOutPut(1, scriptPath.c_str());

	if (followingProcess)
		QueFollowingProcess(new	PipeSend(&theConnection(), followingProcess, pipe_fd));
	else
		QueFollowingProcess(new	PipeSend(&theConnection(), pipe_fd));
	followingProcess = NULL;
	return (false);
}

ExecuteFile::ExecuteFile(Connection *connection, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(connection), followingProcess(NULL), executor(executablePath), scriptPath(scriptPath) {};

ExecuteFile::ExecuteFile(Connection *connection, ServiceProcess *followingProcess, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(connection), followingProcess(followingProcess), executor(executablePath), scriptPath(scriptPath) {};

ExecuteFile::~ExecuteFile()
{
	if (followingProcess)
		delete (followingProcess);
}