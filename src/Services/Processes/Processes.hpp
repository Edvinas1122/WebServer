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
		// FileReceive(FileReceive const &process):
		// 				ServiceProcess(process), fileToReceive(process.fileToReceive), lenght(process.lenght) {};
		// FileReceive(FileReceive const &process, ServiceProcess *followingProcess):
		// 				ServiceProcess(process, followingProcess), fileToReceive(process.fileToReceive), lenght(process.lenght) {};
		virtual ~FileReceive() {};

	virtual bool	Handle();
	void			CreateFile(std::string const &filename);
};

class	PipeSend: virtual public ServiceProcess
{
	private:
		const int	fd;
	public:
		PipeSend(Connection *connection, const int &fd): ServiceProcess(connection), fd(fd), wait(false) {};
		PipeSend(Connection *connection, ServiceProcess *followingProcess, const int &fd): ServiceProcess(connection, followingProcess), fd(fd), wait(false) {};
		~PipeSend() {close(fd);};

	bool Handle();
	private:
	bool	wait;
	void	bufferConnection();
};

#include <Buffer.hpp>

// class	BufferReceive: virtual public ServiceProcess
// {
// 	private:
// 		const int	size;
// 		Buffer		receive;
// 	public:
// 		BufferReceive(Connection *connection, const int size): ServiceProcess(connection) {};
// 		BufferReceive(Connection *connection, ServiceProcess *followingProcess, const int size): ServiceProcess(connection, followingProcess) {};
// 		~BufferReceive() {};

// 	bool Handle();
// };

#include <Executor.hpp>

class	ExecuteFile : virtual public ServiceProcess
{
	private:
		ServiceProcess			*followingProcess;
		Executor 				executor;
		const std::string		scriptPath;
		int						inputToExec;
		int						writeEndToInputOfExec;
		std::list<std::string>	environmnet;

	public:
		ExecuteFile(Connection *connection, std::string const &executablePath,
						std::string const &scriptPath);
		ExecuteFile(Connection *connection, ServiceProcess *followingProcess,
						std::string const &executablePath, std::string const &scriptPath);
		virtual ~ExecuteFile();

	bool	Handle();


	int		WriteBufferToExecutorInput(void *buffer, size_t len);
	void	SetEnvVariable(std::string const &env);

	private:

	int		PipeIntoExec();
};

#endif