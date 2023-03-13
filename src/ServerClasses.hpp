#ifndef SERVERCLASSES_HPP
# define SERVERCLASSES_HPP

# include <includes.hpp>
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
		std::multimap<std::string, std::string> _head;
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

#endif