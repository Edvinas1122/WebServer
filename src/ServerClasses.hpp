#ifndef SERVERCLASSES_HPP
# define SERVERCLASSES_HPP

# include <includes.hpp>
# include <DescendParser.hpp>

enum HttpMethodes
{
	GET,
	POST,
	PUT,
	HEAD,
	DELETE,
	NONE
};

/*
	Set a default file to answer if the request is a directory
	Make it work with POST and GET methods
	Make the route able to accept uploaded files and configure where they should
	be saved.
*/

class Route
{
	private:
		bool		directory_listing_enabled;
		std::string	response_dir;
		std::string	upload_dir;
		std::string	redirect;
		std::string	default_file;
		short		forbit_methods[8];
	public:
		Route() {};
		Route(DescendParser parser);

		void	displayInfo(const char *append) const;
};

/*
	Response types:
		1 string -	text or html or redirect
		2 fd -		file or cgi
		or 
		1 fd - 		
*/

/*
	Limit client body size
	server_names can be missing or not

	Max body size implament

	CGI ?
	atribute validators - design- a bit more completed validation

*/

class VirtualServer {
	private:
		std::string							host;
		std::string							server_name;
		std::string							root_dir;
		std::string							index;
		std::map<std::string, std::string>	cgi_response;
		std::map<std::string, std::string>	error_pages;
		std::list<std::string>				port_number;
		std::map<std::string, Route>		locations;
		size_t								max_body_size;
		// std::map<std::string, CGI>			cgi_map;
	public:
		VirtualServer() {};
		VirtualServer(DescendParser parser);
		// VirtualServer(const VirtualServer &src);
		// VirtualServer& operator=(const VirtualServer& other);
		const char	*getRoot() const;
		Route		getLocation(std::string const &location); // <--- /jumper/tall/ > "null or /jumper/"
		const char	*getHost() const;
		const char	*getServerName() const;
		void		displayInfo() const;

	public:
		static bool	validServerName(std::string const &str);
		static bool	validIndexFile(std::string const &str);
		static bool	pathcheck(std::string const &str);
		static bool	isPositiveNumber(std::string const &str);
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

};

/*
	A complete message to a client
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

class Client {
	public:
		Response		response;
	private:
		HttpRequest		request;
		VirtualServer	*virtualServer;
		sockaddr_in 	socketAddress;
		int				clientFd;
		struct timeval	lst_msg_time;
	public:
		Client() {};
		~Client();
		Client(const int clientFd, const struct sockaddr_in &socketAddress);
		/**/
		const int					&getSocket() const;
		const struct socketaddr_in	&getAddress() const;
		time_t						getElapsedTime() const;
		const HttpRequest			&getRequest() const; // debug purposes

		void						setSocket(int const &socketFd);
		void						setAddress(sockaddr_in const &socketAddress);
		void						updateTime();
		void						setServer(VirtualServer *virtualServer);
		/*
			build response from a request
		*/
		void						buildResponse();
};

class	Observer
{
	private:
        std::vector<struct pollfd>   vector;
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

class	ClientsQue: virtual public Observer
{
	public:
		typedef	std::map<int, Client>	listOfClients;
	private:
		listOfClients	Clients;
	
	protected:

		void	setClients(std::list<int> const &Clients);
		void	removeClient(listOfClients::iterator &position);
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