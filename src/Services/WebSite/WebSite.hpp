#ifndef WEBSITE_HPP
# define WEBSITE_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Processes.hpp>
# include <Connection.hpp>
# include <VirtualServer.hpp>
# include <HTTP.hpp>
# ifndef TEMP_FILES_DIR
#  define TEMP_FILES_DIR "/var/tmp/cgi_tmp"
# endif

class	HTTPParser : virtual public BufferRequest
{
	private:
		VirtualServer		*virtualServer;
		static const int	heartBeatRate = 10000;

	public:
		HTTPParser(Connection *connection, VirtualServer *vs): ServiceProcess(connection), BufferRequest(connection), virtualServer(vs) {};
		HTTPParser(Connection *connection, ServiceProcess *followingProcess, VirtualServer *vs):
					ServiceProcess(connection), BufferRequest(connection, followingProcess),  virtualServer(vs) {};
		HTTPParser(const HTTPParser &src): ServiceProcess(src), BufferRequest(src), virtualServer(src.virtualServer),
											max_receive_size(src.max_receive_size), received(src.received) {};
		HTTPParser(const HTTPParser &src, ServiceProcess *followingProcess):
					ServiceProcess(src, followingProcess), BufferRequest(src, followingProcess),
					virtualServer(src.virtualServer), max_receive_size(src.max_receive_size), received(src.received) {};
		virtual ~HTTPParser() {};

	virtual	bool	Handle();
	bool			HeartBeat();
	protected:

	virtual ServiceProcess	*RequestParse(std::string const &request);
	virtual bool			RequestCompleted(std::string const &request);

	protected:
	virtual ServiceProcess	*ErrorRespone(const int code, bool close_connection = false);

	private:
	ServiceProcess		*handleDeleteRequest(std::string const &dir, HttpRequest const &request);
	ServiceProcess		*handleGetRequest(std::string const &dir, HttpRequest const &request);
	ServiceProcess		*handleUploadRequest(std::string const &dir, HttpRequest const &request);
	ExecuteFile			*handleCGIExecution(std::string const &dir, HttpRequest const &request);

	protected:
		bool	allowInsert(const size_t newInsertSize);
	private:
		void	setMaxReceiveSize(const size_t value);
		size_t	max_receive_size;
		size_t	received;
};

class	HTTPFileSend : public HTTPParser, public FileSend
{
	public:
		HTTPFileSend(const HTTPParser &process, std::string const &path):
						ServiceProcess(process), BufferRequest(process), HTTPParser(process), FileSend(&theConnection(), path) {};
		HTTPFileSend(const HTTPParser &process, ServiceProcess *followingProcess, std::string const &path):
						ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess),
						HTTPParser(process, followingProcess), FileSend(&theConnection(), followingProcess, path) {};
		virtual ~HTTPFileSend() {};

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

class	HTTPBufferReceive : public HTTPParser
{
	protected:
		Buffer				buffer;
		bool				chunkTrimmed;

	public:
		HTTPBufferReceive(const HTTPParser &process):
						ServiceProcess(process), BufferRequest(process), HTTPParser(process), chunkTrimmed(false) {};
		HTTPBufferReceive(const HTTPParser &process, ServiceProcess *followingProcess):
						ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess),
						HTTPParser(process, followingProcess), chunkTrimmed(false) {};
		virtual ~HTTPBufferReceive() {};

	virtual bool	Handle();

	HTTPBufferReceive	&operator<<(Buffer const &src) {
		this->buffer << src;
		return (*this);
	};

	protected:
	void			Continue();
	virtual	bool	CheckChunkHeader() = 0;
	virtual bool	ChunkBeginTrimHandle() = 0;
	virtual bool	CheckChunkEnd() = 0;
	virtual void	ChunkEndHandle() = 0;

	public:
	class	ExceededMaximumLen: public std::exception {};
};

class	WebSite: public Service
{
	private:
		VirtualServers		*virtualServers;
		static const int	timeoutAge = 3000; 
	public:
		WebSite() {};
		~WebSite() {};

	ServiceProcess	*RequestParse(Connection *connection, std::string const &request);
	virtual bool	DetermineIfIdleProcessType(ServiceProcess *process);
	size_t			TimeOutAge() const {
		return (timeoutAge);
	};

	void	SetVirtualServerMap(VirtualServers *virtualServers)
	{
		this->virtualServers = virtualServers;
	};
};

#endif
