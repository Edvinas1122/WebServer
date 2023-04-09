#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <includes.hpp>
# include <stdarg.h>

class	Executor
{
	private:
		const std::string	file;
		const int			input_fd;
		int					output_fd;
		std::vector<char*>	commands;
		std::vector<char*>	environment;

	public:
		Executor(): input_fd(STDIN_FILENO), output_fd(STDOUT_FILENO) {};
		Executor(std::string const &path);
		Executor(std::string const &path, const int &in_fd, const int &out_fd);
		~Executor();

	void	execute(std::list<std::string> const &param);
	void	execute(int count, ...);
	void	execute();
	int		executeToOutPut();

	void	setEnv(std::list<std::string> const &param);
	void	setEnv(int count, ...);

	private:
	void	execute(std::string const &param);
	void	setEnv(std::string const &param);

	char *const	*getCommand();
	char *const *getEnv();

	void	setCommandParam(std::string const &command);
	void	setCommandParams(std::list<std::string> const &param);
	void	clearCommands();

};

#endif
