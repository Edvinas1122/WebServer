#include "Processes.hpp"

void	deleteTempFile(ServiceProcess *currentProcess, ServiceProcess *followingProcess, Connection *connection)
{
	FileSend	*send = dynamic_cast<FileSend*>(currentProcess);

	(void) followingProcess;
	(void) connection;

	if (send)
		send->removeFile();
	else
		throw std::exception();
}

bool	ExecuteFile::Handle()
{
	if (!waiting) 
	{
		try {
			callExecution();
		} catch (Executor::ExecutionFailed &e) {
			SetFollowingProcess(followingProcess);
			followingProcess = NULL;
			return (false);
		}
		startClock();
	}
	if (timedOut())
	{
		executor.killProcess();
		throw InfiniteLoop();
		return (false);
	}
	if (executor.hasFinished())
	{
		deliverOutPut();
		return (false);
	}
	return (true);
}

void	ExecuteFile::startClock()
{
	waiting = true;
	gettimeofday(&timestamp, NULL);	
}

bool	ExecuteFile::timedOut()
{
	struct timeval	currenttime;

	gettimeofday(&currenttime, NULL);	
	if (currenttime.tv_sec - timestamp.tv_sec > 2)
		return (true);
	return (false);
}

void	ExecuteFile::deliverOutPut()
{
	FileSend		*newFollowingProcess;

	if (followingProcess && OutputFileName.empty())
		SetFollowingProcess(new	PipeSend(&theConnection(), followingProcess, executionOutput_read_end));
	else if (OutputFileName.empty())
		SetFollowingProcess(new	PipeSend(&theConnection(), executionOutput_read_end));
	else if (followingProcess)
	{
		newFollowingProcess = new FileSend(&theConnection(), followingProcess, OutputFileName);
		newFollowingProcess->ScheduleFollowUp(deleteTempFile); // promise to delete a file after sending
		SetFollowingProcess(newFollowingProcess);
	}
	else
	{
		newFollowingProcess = new FileSend(&theConnection(), OutputFileName);
		newFollowingProcess->ScheduleFollowUp(deleteTempFile); // promise to delete a file after sending
		SetFollowingProcess(newFollowingProcess);
	}
	followingProcess = NULL;
}

void	ExecuteFile::callExecution()
{
	if (!environmnet.empty())
		executor.setEnv(environmnet);
	executor.setInputFd(DirrectFileIntoExec());
	if (OutputFileName.empty())
		executionOutput_read_end = executor.executeToOutPut(1, scriptPath.c_str());
	else
		executionOutput_read_end = executor.executeToFile(2, scriptPath.c_str(), OutputFileName.c_str());
}

ExecuteFile::ExecuteFile(Connection *connection, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(connection), followingProcess(NULL), executor(executablePath), scriptPath(scriptPath), inputToExec(STDIN_FILENO), writeEndToInputOfExec(0), waiting(false) {};

ExecuteFile::ExecuteFile(Connection *connection, ServiceProcess *followingProcess, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(connection), followingProcess(followingProcess), executor(executablePath), scriptPath(scriptPath), inputToExec(STDIN_FILENO), writeEndToInputOfExec(0), waiting(false) {};

ExecuteFile::ExecuteFile(ServiceProcess const &process, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(process), followingProcess(NULL), executor(executablePath), scriptPath(scriptPath), inputToExec(STDIN_FILENO), writeEndToInputOfExec(0), waiting(false) {};

ExecuteFile::ExecuteFile(ServiceProcess const &process, ServiceProcess *followingProcess, std::string const &executablePath, std::string const &scriptPath):
						ServiceProcess(process), followingProcess(followingProcess), executor(executablePath), scriptPath(scriptPath), inputToExec(STDIN_FILENO), writeEndToInputOfExec(0), waiting(false) {};

ExecuteFile::~ExecuteFile()
{
	if (followingProcess)
		delete (followingProcess);
	if (!filename.empty())
		std::remove(filename.c_str());
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

int	ExecuteFile::DirrectFileIntoExec()
{
	if (!filename.empty())
	{
		inputToExec = open(filename.c_str(), O_RDWR, 0666);
		if (inputToExec  == -1)
			throw std::exception();
	}
	return (inputToExec);
}

void	ExecuteFile::OutputToFile(std::string const &newFileName)
{
	OutputFileName = newFileName;
}

void	ExecuteFile::FileIntoExec(std::string const &filePath)
{
	filename = filePath;
}