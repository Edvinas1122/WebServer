#include "Executor.hpp"

int	main()
{
	Executor	executor("/usr/bin/cat", open("/home/WebServer/http/files/gay.txt", O_RDONLY), STDOUT_FILENO);

	executor.setEnv(2, "PWD=/var/www", "HOME=/root");
	// executor.execute(2, "-l", "/home");
	// executor.execute(1, "-l", "/");
	int pipe_read = executor.executeToOutPut();
	char	buf[300];

	memset(buf, 0, 300);
	read(pipe_read, buf, 300);
	std::cout << buf << std::endl;
	close(pipe_read);
	return (0);
}