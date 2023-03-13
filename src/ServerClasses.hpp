#ifndef SERVERCLASSES_HPP
# define SERVERCLASSES_HPP

# include <includes.hpp>
# include <Terminal.hpp>
# include <VirtualServer.hpp>
# include <Parser.hpp>
# include <Error.hpp>

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
		HttpMethodes method;
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
		std::string body;
		int			error;

	public:
		HttpRequest() {};
		HttpRequest(Parser parser);
		int getError(){return(error);}
		size_t getBodylength(){return(body.length());}
		std::string getBody(){return(body);}
		HttpMethodes getMethod(){return(method);}
};


/*
	A complete message to a client
	https://www.tutorialspoint.com/http/http_requests.html
*/
class Response
{
private:
	std::string version;
	std::multimap<std::string, std::string> 	_head;
	std::string _body;
	std::string _location;
	std::string _response;
	int _status;
	size_t _content_length;
	HttpRequest _request;

	size_t		_cgi_response_lenght;

	void build_error(int code);
	bool buildBody();
	void getLocationMatch();
	bool handleTarget();
	bool reqError();
	void setStatusLine();
	void setHeaders();
public:
	Response();
	//Response(const HttpRequest &);
	//Response &operator=(const HttpRequest &);
	//Response(HttpRequest &);
	//Response &operator=(HttpRequest &);
	//~Response();
	void		Build();
	std::string	get(){return (_response);}
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
		void		sendPacket();

		bool		ready() const; // outgoing empty

		std::string	getMessage() const {
			return (incoming);
		};

		Tcp	&operator<<(const std::string& str);

		friend std::ostream& operator<<(std::ostream &os, Tcp &obj)
		{
			if (obj.incoming.length()) {
				os << obj.incoming;
				obj.flushIncoming();
			}
			return os;
		}
	private:
		void flushIncoming() {
			incoming.clear();
		}
};

class	Client: public Tcp {
	// public:
		// Response		response;
	private:
		// HttpRequest		request;
		sockaddr_in 	socketAddress;
		struct timeval	lst_msg_time;

	public:
		bool			serviceStatus;
	public:
		Client() {serviceStatus = true;};
		~Client();
		Client(const int clientFd, const struct sockaddr_in &socketAddress);

		bool	responseBuilt()
		{
			return (serviceStatus);
		};

		/**/
		const int					&getSocket() const;
		const struct socketaddr_in	&getAddress() const;
		time_t						getElapsedTime() const;
		// const HttpRequest			&getRequest() const; // debug purposes

		// bool						ProcessMessage();
		void						setSocket(int const &socketFd);
		void						setAddress(sockaddr_in const &socketAddress);
		void						updateTime(const bool timeout = false);
		void						setServer(VirtualServer *virtualServer);
		/*
			build response from a request
		*/
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

		VirtualServer	&getServer(std::string const &port, std::string const &host) const;

	// protected:
		// void parseServers(DescendParser &parser);

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

		void	insertFileDescriptor(const int fd, const int events = POLLIN | POLLOUT, const bool asynch = true);
		void	removeFileDescriptor(const int fd);
		bool	checkFd(const int fd, const int events = POLLIN);

	private:
		void	setAsNonBlocking(const int socket_fd) const;

	public:
		class	InvalidFd: public std::exception {};
};

class	PortSockets: virtual public Observer
{
	public:
		typedef std::map<std::string, int>	socketMap;	
	private:
		socketMap	portSockets;
		int	(*openPortSocket)(char const *);
	public:
		PortSockets() {
			this->openPortSocket = default_open_port;
		};
		~PortSockets() {};

		template <typename PARSER>
		void	startPorts(std::list<std::string> (*parsingMethod)(PARSER &), PARSER parser, bool asynch = true)
		{
			std::list<std::string>::iterator	it;
			std::list<std::string>				socketList;
			int									fd;

			socketList = parsingMethod(parser);
			it = socketList.begin();
			while (it != socketList.end())
			{
				if (portSockets.find(*it) == portSockets.end())
				{
					fd = this->openPortSocket(it->c_str());
					portSockets[*it] = fd;
					insertFileDescriptor(fd, POLLIN, asynch);
				}
				it++;
			}
		};
	protected:

		std::list<int>	getSockets();
		std::list<int>	getLoudSockets(const int events = POLLIN);

	public:

		void	setStartSocketMethod(int (*openPortSocket)(char const *))
		{
			this->openPortSocket = openPortSocket;
		};

	private:
		static int	default_open_port(char const *port){
			(void) (port);
			return (0);
		};
	
};

class	ConnectionQue: virtual public Observer
{
	public:
		typedef	std::map<int, Client>	listOfClients;
	private:
		listOfClients	Clients;

	public:
		ConnectionQue() {};
		~ConnectionQue() {};
	protected:

		void	setClients(std::list<int> const &Clients);
		void	closeConnection(listOfClients::iterator &position);
		void	closeTimeOut();

		void	queProcess(bool (*action)(Client &client), const int observer_event = POLLIN);
	
		void	action(void (*action)(Client &client));

		template <typename TYPE>
		void	action(void (*action)(Client &, TYPE), TYPE insertion);

};

template <typename TYPE>
void	ConnectionQue::action(void (*action)(Client &, TYPE), TYPE insertion)
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		action(it->second, insertion);
		it++;
	}
}

template<typename TYPE = std::string>
class	Server: public ConnectionQue, public PortSockets
{
	private:
		void (*receivingAction)(Client &client);
		void (*respondingAction)(Client &client);
		void (*additionalAction)(Client &, TYPE);

	public:
		Server() {
			receivingAction = default_action;
			respondingAction = default_action;
		};
		~Server() {};
	
		void	setReceivingAction(void (*receivingAction)(Client &client))
		{
			this->receivingAction = receivingAction;
		};

		void	setRespondingAction(void (*respondingAction)(Client &client))
		{
			this->respondingAction = respondingAction;
		};

		void	Run();

	private:
		static bool	pullIncoming(Client &client);
		static bool	pushOutgoing(Client &client);
		static void	default_action(Client &client) {
			(void) client;
		};
};

template<typename TYPE>
void	Server<TYPE>::Run()
{
	Observer::Poll(true);

	ConnectionQue::setClients(getLoudSockets());

	ConnectionQue::queProcess(pullIncoming, POLLIN);
	ConnectionQue::action(receivingAction);
	// ClientsQue::action(buildResponse);
	// terminal_interface();
	// ClientsQue::action(respondingAction, extractMessage());
	// clearMessage();

	ConnectionQue::queProcess(pushOutgoing, POLLOUT);
	ConnectionQue::closeTimeOut();
}

template<typename TYPE>
bool	Server<TYPE>::pullIncoming(Client &client)
{
	client.receivePacket();
	client.serviceStatus = false;
	return (true);
}

#define	CLOSE_CLIENT true

template<typename TYPE>
bool	Server<TYPE>::pushOutgoing(Client &client)
{
	if (client.ready())
	{
		try {
			client.sendPacket();
			return (true);
		} catch(const std::exception& e) {
			client.updateTime(CLOSE_CLIENT);
			return (false);
		}	
	}
	return (false);
}

#endif