#ifndef SERVERCLASSES_HPP
# define SERVERCLASSES_HPP

# include <includes.hpp>
# include <DescendParser.hpp>
# include <Terminal.hpp>
# include <VirtualServer.hpp>
# include <Parser.hpp>

enum HttpMethodes
{
	GET,
	POST,
	PUT,
	HEAD,
	DELETE,
	NONE
};

class HttpRequest {
	private:
		size_t		method;
		size_t		protocol_version;
		bool		keep_alive;
		std::string	host;
		std::string	path; // full query
		/*
			query parsed into:
		*/
		std::string	location;
		std::string	file_name;
		std::string	query;

	public:
		HttpRequest() {};
		HttpRequest(Parser parser) {

			std::cout << parser.getValue("Dog") << std::endl;
		};
};

/*
	A complete message to a client
	https://www.tutorialspoint.com/http/http_requests.html
*/
class Response {
	private:
		HttpRequest	request;
		
	public:
		bool		text;
		std::string	getRes();
		// std::string getResHeader();
		// int			getBodyFd();
		void		setRequest(HttpRequest &);
		void		build();
		// /*some data type*/	get();
		
};

class	Tcp
{
	private:
		int			fd;
		std::string	incoming;
		std::string	outgoing;
	public:
		Tcp() {};
		Tcp(const int &fd): fd(fd) {};

	public:
		void		receivePacket();
		bool		ready() const;
		void		sendPacket();

		std::string	getMessage() const {
			return (incoming);
		};

		Tcp	&operator<<(const std::string& str);
};

template<typename PARSER>
class	MessageProcessor
{
	private:
		bool	state;
	public:
		MessageProcessor(): state(false) {};

		std::string	validateFormat(std::string const &message);

		template<typename TYPE>
		TYPE		getParsed(std::string const &message) {
			return (TYPE(PARSER(message)));
		};

		bool		processState()
		{
			return (!state);
		};

	private:
		
};


class	Client: public Tcp, public MessageProcessor<Parser> {
	public:
		Response		response;
	private:
		HttpRequest		request;
		VirtualServer	*virtualServer;
		sockaddr_in 	socketAddress;
		struct timeval	lst_msg_time;
	public:
		Client() {};
		~Client();
		Client(const int clientFd, const struct sockaddr_in &socketAddress);

		/**/
		const int					&getSocket() const;
		const struct socketaddr_in	&getAddress() const;
		time_t						getElapsedTime() const;
		// const HttpRequest			&getRequest() const; // debug purposes

		// bool						ProcessMessage();
		void						setSocket(int const &socketFd);
		void						setAddress(sockaddr_in const &socketAddress);
		void						updateTime();
		void						setServer(VirtualServer *virtualServer);
		/*
			build response from a request
		*/
		// void						BuildResponse();
		void						ProcessMessage();
};

class	Observer
{
	private:
        std::vector<struct pollfd>	vector;
		static const int MAX_CONNECTIONS = 10;
		static const int TIMEOUT = 5000;
	public:
		Observer();
		~Observer() {};

		int	Poll(const bool timeOutOn = false);

	protected:

		void	insertFileDescriptor(const int fd, const int events = POLLIN | POLLOUT);
		void	removeFileDescriptor(const int fd);
		bool	checkFd(const int fd, const int events = POLLIN);

	private:
		void	setAsNonBlocking(const int socket_fd) const;

	public:
		class	InvalidFd: public std::exception {};
};

class	ClientsQue: virtual public Observer, public Terminal
{
	public:
		typedef	std::map<int, Client>	listOfClients;
	private:
		listOfClients	Clients;
	
	protected:

		void	setClients(std::list<int> const &Clients);
		void	removeClient(listOfClients::iterator &position);
		void	readClients();
		void	respondClients();
		void	closeTimeOut();

};

class	PortSockets: virtual public Observer
{
	private:
		std::map<std::string, int>	portSockets;
	
	public:
		PortSockets() {};
		~PortSockets() {};

	protected:
		void startPorts(DescendParser &parser, bool asynch = true);
		std::list<int>	getLoudSockets(const int events = POLLIN);
		std::list<int>	getSockets();
	
};

class	VirtualServers {
	public:
		typedef	std::map<std::string, VirtualServer>	virtualServerMap;
	protected:
		virtualServerMap	virtualServers;
	
	public:
		VirtualServers() {};
		~VirtualServers() {};

		void	Info() const;

	protected:
		void parseServers(DescendParser &parser);

};

class	ServerManager: public ClientsQue, public PortSockets, public VirtualServers
{
	public:
		void	serverCreator(const char *config_file);
		void	Start();

	private:
		// void	receiveRequest();
		// void	respondToClient();
};

#endif