#ifndef SERVERCLASSES_HPP
# define SERVERCLASSES_HPP

# include <includes.hpp>
# include <DescendParser.hpp>
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
		HttpRequest(Parser parser) {
			(void) parser;
		};
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
		bool		ready() const;
		void		sendPacket();


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
		void						updateTime(const bool timeout = false);
		void						setServer(VirtualServer *virtualServer);
		/*
			build response from a request
		*/
		// void						BuildResponse(VirtualServer	*virtualServer);
		void						BuildResponse();
		
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

		VirtualServer	&getServer(std::string const &port, std::string const &host) const;

	protected:
		void parseServers(DescendParser &parser);

};

// #define BYPASS_OBSERVER 42

class	ClientsQue: virtual public Observer
{
	public:
		typedef	std::map<int, Client>	listOfClients;
	private:
		listOfClients	Clients;

	public:
		ClientsQue() {};
		~ClientsQue() {};
	protected:

		void	setClients(std::list<int> const &Clients);
		void	removeClient(listOfClients::iterator &position);
		void	closeTimeOut();

		void	queProcess(bool (*action)(Client &client), const int observer_event = POLLIN);
	
		void	action(void (*action)(Client &client));

		template <typename TYPE>
		void	action(void (*action)(Client &, TYPE), TYPE insertion);

};

template <typename TYPE>
void	ClientsQue::action(void (*action)(Client &, TYPE), TYPE insertion)
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		action(it->second, insertion);
		it++;
	}
}

class	Server: public ClientsQue, public VirtualServers
{
	public:
		Server() {};
		~Server() {};
	
		// template <typename TYPE>
		// void	action(void (*action)(Client &client), TYPE insertion);
};

class	ServerManager: public Server, public PortSockets, public Terminal
{
	public:
		void	serverCreator(const char *config_file);
		void	Start();

	private:
		// void	receiveRequest();
		// void	respondToClient();
};
#endif