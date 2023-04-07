#ifndef WEBSITE_HPP
# define WEBSITE_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Processes.hpp>
# include <Connection.hpp>
# include <VirtualServer.hpp>
# include <HTTP.hpp>

class	HTTPParser : virtual public MasterProcess
{
	private:
		VirtualServers	*virtualServers;
		static const int	heartBeatRate = 10;

	public:
		HTTPParser(Connection *connection, VirtualServers *vs): ServiceProcess(connection), MasterProcess(connection), virtualServers(vs) {};
		HTTPParser(Connection *connection, ServiceProcess *followingProcess, VirtualServers *vs):
					ServiceProcess(connection), MasterProcess(connection, followingProcess),  virtualServers(vs) {};
		HTTPParser(const HTTPParser &src): ServiceProcess(src), MasterProcess(src), virtualServers(src.virtualServers) {};
		HTTPParser(const HTTPParser &src, ServiceProcess *followingProcess):
					ServiceProcess(src, followingProcess), MasterProcess(src, followingProcess), virtualServers(src.virtualServers) {};
		virtual ~HTTPParser() {};

	virtual	bool	Handle();
	bool			HeartBeat();
	protected:

	virtual ServiceProcess	*RequestParse(std::string const &request);

	private:
	
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

class	HTTPFileReceiveReport : public ServiceProcess
{
	public:
		HTTPFileReceiveReport(const ServiceProcess &src): ServiceProcess(src) {};
		HTTPFileReceiveReport(const ServiceProcess &src, ServiceProcess *followingProcess): ServiceProcess(src, followingProcess) {};
		virtual ~HTTPFileReceiveReport() {};

	bool	Handle();
};

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

class	WebSite: public Service
{
	private:
		VirtualServers	*virtualServers;
	public:
		WebSite() {};
		~WebSite() {};

	ServiceProcess	*RequestParse(Connection *connection, std::string const &request);
	virtual bool	DetermineIfIdleProcessType(ServiceProcess *process);
	size_t			TimeOutAge() const {
		return (30);
	};
};

#endif