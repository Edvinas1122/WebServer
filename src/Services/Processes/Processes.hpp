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

# include <HTTP.hpp>
# include <VirtualServer.hpp>

class	HTTPParser : virtual public MasterProcess
{
	private:
		VirtualServers	*virtualServers;

	public:
		HTTPParser(Connection *connection, VirtualServers *vs): ServiceProcess(connection), MasterProcess(connection), virtualServers(vs) {};
		HTTPParser(Connection *connection, ServiceProcess *followingProcess, VirtualServers *vs):
					ServiceProcess(connection), MasterProcess(connection, followingProcess),  virtualServers(vs) {};
		HTTPParser(const HTTPParser &src): ServiceProcess(src), MasterProcess(src), virtualServers(src.virtualServers) {};
		HTTPParser(const HTTPParser &src, ServiceProcess *followingProcess):
					ServiceProcess(src, followingProcess), MasterProcess(src, followingProcess), virtualServers(src.virtualServers) {};
		virtual ~HTTPParser() {};

	virtual	bool	Handle();
	bool			HeartBeatIdleConnection();
	protected:

	virtual ServiceProcess	*RequestParse(std::string const &request);
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

class	HTTPFileSend : public HTTPParser, public FileSend
{
	public:
		HTTPFileSend(const HTTPParser &process, std::string const &path):
						ServiceProcess(process), MasterProcess(process), HTTPParser(process), FileSend(&theConnection(), path) {};
		HTTPFileSend(const HTTPParser &process, ServiceProcess *followingProcess, std::string const &path):
						ServiceProcess(process, followingProcess), MasterProcess(process, followingProcess),
						HTTPParser(process, followingProcess), FileSend(&theConnection(), followingProcess, path) {};
		virtual ~HTTPFileSend() {};

	bool	Handle();
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

class	HTTPFileReceiveReport : public ServiceProcess
{
	public:
		HTTPFileReceiveReport(const ServiceProcess &src): ServiceProcess(src) {};
		HTTPFileReceiveReport(const ServiceProcess &src, ServiceProcess *followingProcess): ServiceProcess(src, followingProcess) {};
		virtual ~HTTPFileReceiveReport() {};

	bool	Handle();
};

#include <Connection.hpp>

class	HTTPFileReceive : public HTTPParser, public FileReceive
{
	private:
		Buffer				buffer;
		const std::string	delimiter;
	public:
		HTTPFileReceive(const HTTPParser &process, std::string const &path, size_t const &len, std::string const &delimiter):
						ServiceProcess(process), MasterProcess(process), HTTPParser(process), FileReceive(&theConnection(), path, len), delimiter(delimiter) {};
		HTTPFileReceive(const HTTPParser &process, ServiceProcess *followingProcess, std::string const &path, size_t const &len, std::string const &delimiter):
						ServiceProcess(process, followingProcess), MasterProcess(process, followingProcess), HTTPParser(process, followingProcess),
						FileReceive(&theConnection(), followingProcess, path, len), delimiter(delimiter) {};
		virtual ~HTTPFileReceive() {};

	bool	Handle();

	HTTPFileReceive	&operator<<(Buffer &src) {
		this->buffer << src;
		return (*this);
	};
};

#endif