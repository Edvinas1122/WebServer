#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <includes.hpp>
# include <Error.hpp>
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

# include <VirtualServer.hpp>

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