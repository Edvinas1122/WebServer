#include "Processes.hpp"

bool	ExecuteFile::Handle()
{
	int	executionOutput_read_end;

	if (!environmnet.empty())
		executor.setEnv(environmnet);
	executor.setInputFd(inputToExec);
	try {
		executionOutput_read_end = executor.executeToOutPut(1, scriptPath.c_str());
	} catch (Executor::ExecutionFailed &e) {
		SetFollowingProcess(followingProcess);
		followingProcess = NULL;
		return (false);
	}
	if (followingProcess)
		SetFollowingProcess(new	PipeSend(&theConnection(), followingProcess, executionOutput_read_end));
	else
		SetFollowingProcess(new	PipeSend(&theConnection(), executionOutput_read_end));
	followingProcess = NULL;
	return (false);
}

ExecuteFile::ExecuteFile(Connection *connection, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(connection), followingProcess(NULL), executor(executablePath), scriptPath(scriptPath), inputToExec(STDIN_FILENO), writeEndToInputOfExec(0) {};

ExecuteFile::ExecuteFile(Connection *connection, ServiceProcess *followingProcess, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(connection), followingProcess(followingProcess), executor(executablePath), scriptPath(scriptPath), inputToExec(STDIN_FILENO), writeEndToInputOfExec(0) {};

ExecuteFile::~ExecuteFile()
{
	if (followingProcess)
		delete (followingProcess);
}

int	ExecuteFile::WriteBufferToExecutorInput(void *buffer, size_t len)
{
	if (!writeEndToInputOfExec)
		writeEndToInputOfExec = PipeIntoExec();
	return (write(writeEndToInputOfExec, buffer, len));
}

void	ExecuteFile::SetEnvVariable(std::string const &env)
{
	environmnet.push_back(env);
}

int	ExecuteFile::PipeIntoExec()
{
	int	pipe_fd[2];

	if (pipe(pipe_fd))
		throw std::exception();
	inputToExec = pipe_fd[0];
	return (pipe_fd[1]);
}