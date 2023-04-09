#ifndef PROCESSES_HPP
# define PROCESSES_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <File.hpp>

class	TerminateProcess: public ServiceProcess
{
	public:
		TerminateProcess(Connection *connection): ServiceProcess(connection) {};
		TerminateProcess(TerminateProcess const &src): ServiceProcess(src) {};
		virtual ~TerminateProcess() {};

	bool	Handle() {throw (std::exception());};
};

class	MasterProcess : virtual public ServiceProcess
{
	public:
		MasterProcess(Connection *connection): ServiceProcess(connection) {};
		MasterProcess(Connection *connection, ServiceProcess *followingProcess):
						ServiceProcess(connection, followingProcess) {};
		MasterProcess(const MasterProcess &src): ServiceProcess(src) {};
		MasterProcess(const MasterProcess &src, ServiceProcess *followingProcess):
						ServiceProcess(src, followingProcess) {};
		virtual ~MasterProcess() {};

	virtual bool	Handle();

	protected:

	virtual ServiceProcess	*RequestParse(std::string const &request) = 0;
};

class	FileSend : virtual public ServiceProcess
{
	private:
		File	fileToSend;
	public:
		FileSend(Connection *connection, std::string const &path): ServiceProcess(connection), fileToSend(path.c_str()) {};
		FileSend(Connection *connection, ServiceProcess *followingProcess, std::string const &path):
					ServiceProcess(connection, followingProcess), fileToSend(path.c_str()) {};
		virtual ~FileSend() {};

	virtual bool	Handle();
};

class	FileReceive : virtual public ServiceProcess
{
	protected:
		File	fileToReceive;
		size_t	lenght;
	public:
		FileReceive(Connection *connection, std::string const &path, size_t const &len):
						ServiceProcess(connection), fileToReceive(path.c_str()), lenght(len) {
			fileToReceive.Create();
		};
		FileReceive(Connection *connection, ServiceProcess *followingProcess, std::string const &path, size_t const &len):
						ServiceProcess(connection, followingProcess), fileToReceive(path.c_str()), lenght(len) {
			fileToReceive.Create();
		};
		virtual ~FileReceive() {};

	virtual bool	Handle();
};

class	PipeSend: virtual public ServiceProcess
{
	private:
		const int	fd;
	public:
		PipeSend(Connection *connection, const int &fd): ServiceProcess(connection), fd(fd), wait(false) {};
		PipeSend(Connection *connection, ServiceProcess *followingProcess, const int &fd): ServiceProcess(connection, followingProcess), fd(fd), wait(false) {};
		~PipeSend() {};

	bool Handle();
	private:
	bool	wait;
	void	bufferConnection();
};

#include <Executor.hpp>

class	ExecuteFile : virtual public ServiceProcess
{
	private:
		ServiceProcess	*followingProcess;
		Executor 		executor;
	public:
		ExecuteFile(Connection *connection, std::string const &path):
						ServiceProcess(connection), followingProcess(NULL), executor(path) {};
		ExecuteFile(Connection *connection, ServiceProcess *followingProcess, std::string const &path):
						ServiceProcess(connection), followingProcess(followingProcess), executor(path, open("/home/WebServer/http/files/gay.txt", O_RDONLY), STDOUT_FILENO) {};
		virtual ~ExecuteFile();

	bool Handle();
};

#endif