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

class	BufferRequest : virtual public ServiceProcess
{
	public:
		BufferRequest(Connection *connection): ServiceProcess(connection) {};
		BufferRequest(Connection *connection, ServiceProcess *followingProcess):
						ServiceProcess(connection, followingProcess) {};
		BufferRequest(const BufferRequest &src): ServiceProcess(src) {};
		BufferRequest(const BufferRequest &src, ServiceProcess *followingProcess):
						ServiceProcess(src, followingProcess) {};
		virtual ~BufferRequest() {};

	virtual bool	Handle();

	protected:

	virtual ServiceProcess	*RequestParse(std::string const &request) = 0;
	virtual bool			RequestCompleted(std::string const &request) = 0;

	private:

	std::string	requestBuffer;
};

class	FileSend : virtual public ServiceProcess
{
	private:
		File	fileToSend;
	public:
		FileSend(Connection *connection, std::string const &path): ServiceProcess(connection), fileToSend(path.c_str()), filename(path) {};
		FileSend(Connection *connection, ServiceProcess *followingProcess, std::string const &path):
					ServiceProcess(connection, followingProcess), fileToSend(path.c_str()), filename(path) {};
		FileSend(ServiceProcess const &process, std::string const &path): ServiceProcess(process), fileToSend(path.c_str()), filename(path) {};
		FileSend(ServiceProcess const &process, ServiceProcess *followingProcess, std::string const &path):
					ServiceProcess(process, followingProcess), fileToSend(path.c_str()), filename(path) {};
		virtual ~FileSend() {};

	virtual bool	Handle();
	void			removeFile();
	/*
		store file content in buffer and with function return needed parameter
	*/
	size_t	GetBuffer(char*, size_t len = 50);

	private:
		const std::string	filename;
		// std::streamsize		file_len;
};

class	FileReceive : virtual public ServiceProcess
{
	protected:
		File	fileToReceive;
		size_t	lenght;
	public:
		FileReceive(Connection *connection): ServiceProcess(connection) {};
		FileReceive(Connection *connection, ServiceProcess *followingService):
						ServiceProcess(connection, followingService) {};
		FileReceive(FileReceive const &src): ServiceProcess(src) {};
		FileReceive(FileReceive const &src, ServiceProcess *followingService): ServiceProcess(src, followingService) {};
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
	void			CreateFile(std::string const &filename);
};

class	PipeSend: virtual public ServiceProcess
{
	private:
		const int	fd;
	public:
		PipeSend(Connection *connection, const int &fd): ServiceProcess(connection), fd(fd), wait(false)
		{memset(&timestamp, 0, sizeof(struct timeval));};
		PipeSend(Connection *connection, ServiceProcess *followingProcess, const int &fd): ServiceProcess(connection, followingProcess), fd(fd), wait(false)
		{memset(&timestamp, 0, sizeof(struct timeval));};
		~PipeSend() {close(fd);};

	bool Handle();
	private:
	bool	wait;
	void	bufferConnection();
	bool	allowedTimePassed();
	private:
		struct timeval	timestamp;
};

#include <Executor.hpp>

class	ExecuteFile : virtual public ServiceProcess
{
	private:
		ServiceProcess			*followingProcess;
		Executor 				executor;
		const std::string		scriptPath;
		std::string				OutputFileName;
		int						inputToExec;
		int						writeEndToInputOfExec;
		std::list<std::string>	environmnet;

	public:
		ExecuteFile(Connection *connection, std::string const &executablePath,
						std::string const &scriptPath);
		ExecuteFile(Connection *connection, ServiceProcess *followingProcess,
						std::string const &executablePath, std::string const &scriptPath);
		ExecuteFile(ServiceProcess const &process, std::string const &executablePath, std::string const &scriptPath);
		ExecuteFile(ServiceProcess const &process, ServiceProcess *followingProcess, std::string const &executablePath, std::string const &scriptPath);
		virtual ~ExecuteFile();

	bool	Handle();

	/*
		When filepath provided does use a file to temporary store contents else pipe,
		but has to be buffered at once
	*/
	void	SetEnvVariable(std::string const &env);
	void	FileIntoExec(std::string const &filePath);
	void	OutputToFile(std::string const &newFileName);

	private:

	void		startClock();
	bool		timedOut();
	void		callExecution();
	void		deliverOutPut();
	int			PipeIntoExec();
	int			DirrectFileIntoExec();
	std::string	filename;
	bool		waiting;
	int			executionOutput_read_end;
	struct timeval	timestamp;

	public:
	class	InfiniteLoop: public std::exception {};

};

#endif