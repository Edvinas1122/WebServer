#include "Executor.hpp"

Executor::Executor(std::string const &path, const int &in_fd, const int &out_fd): executablePath(path), input_fd(in_fd), output_fd(out_fd)
{
	if (access(path.c_str(), X_OK))
		throw std::exception();
	setCommandParam(path.substr(path.find_last_of("/") + 1));
};

Executor::~Executor()
{
	std::vector<char*>::iterator	it = commands.begin();

	while (it != commands.end())
	{
		delete[] (*it);
		it++;
	}
	it = environment.begin();
	while (it != environment.end())
	{
		delete[] (*it);
		it++;
	}
};

void	Executor::execute(std::string const &param)
{
	setCommandParam(param);
	execute();
}

void	Executor::execute(std::list<std::string> const &params)
{
	setCommandParams(params);
	execute();
}

void	Executor::execute(int count, ...)
{
	va_list	args;

	va_start(args, count);
    while (count--)
		setCommandParam(va_arg(args, const char*));
	va_end(args);
	execute();
}


void	Executor::execute()
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
		dup2(output_fd, STDOUT_FILENO);
		execve(executablePath.c_str(), getCommand(), getEnv());
		std::cerr << "Exec Command Failure in Forked sub process, ending forked sub Process.." << std::endl;
		close(output_fd);
		exit(EXIT_FAILURE);
	} else {
		// write(pipe_fd[1], input_data, strlen(input_data));

	}
	waitpid(pid, &status, 0);
	clearCommands();
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (WEXITSTATUS(status)) {
		close(output_fd);
		throw ExecutionFailed();
	}
}

int		Executor::executeToOutPut(int count, ...)
{
	int	pipe_fd[2];
	va_list	args;

	va_start(args, count);
    while (count--)
		setCommandParam(va_arg(args, const char*));
	va_end(args);
	pipe(pipe_fd);
	output_fd = pipe_fd[1];
	execute();
	return (pipe_fd[0]);
}

/*
	last param filename
*/
int		Executor::executeToFile(int count, ...)
{
	va_list	args;
	std::string	filename;

	va_start(args, count);
    while (count-- > 1)
		setCommandParam(va_arg(args, const char*));
	va_end(args);
	output_fd = open(va_arg(args, const char*), O_WRONLY | O_CREAT , 0666);
	execute();
	return (output_fd);
}

int		Executor::executeToOutPut()
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	output_fd = pipe_fd[1];
	execute();
	return (pipe_fd[0]);
}

void	Executor::setCommandParam(std::string const &command)
{
	char *cmd = new char[command.length() + 1];

	strcpy(cmd, command.c_str());
	commands.push_back(cmd);
}

void	Executor::setCommandParams(std::list<std::string> const &param)
{
	std::list<std::string>::const_iterator	it = param.begin();

	while (it != param.end())
	{
		setCommandParam(*it);
		it++;
	}
}

char *const	*Executor::getCommand()
{
	commands.push_back(NULL);
	return ((char* const*)commands.data());
}

void	Executor::setEnv(std::string const &param)
{
	char *env_param = new char[param.length() + 1];

	strcpy(env_param, param.c_str());
	environment.push_back(env_param);
}

void	Executor::setEnv(std::list<std::string> const &param)
{
	std::list<std::string>::const_iterator	it = param.begin();

	while (it != param.end())
	{
		setEnv(*it);
		it++;
	}
	environment.push_back(NULL);
}

void	Executor::setEnv(int count, ...)
{
	va_list	args;

	va_start(args, count);
    while (count--)
		setEnv(va_arg(args, const char*));
	va_end(args);
	environment.push_back(NULL);
}

char *const	*Executor::getEnv()
{
	return ((char* const*)environment.data());
}

void	Executor::clearCommands()
{
	std::vector<char*>::iterator	it = commands.begin();

	while (it != commands.end())
	{
		delete[] (*it);
		it++;
	}
	commands.clear();
	setCommandParam(executablePath.substr(executablePath.find_last_of("/") + 1));
}
