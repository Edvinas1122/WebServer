#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <includes.hpp>

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
		// HttpRequest _request;

		size_t		_cgi_response_lenght;

		void build_error(int code);
		bool buildBody();
		void getLocationMatch();
		bool handleTarget();
		bool reqError();
		void setStatusLine();

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

template<typename REQUEST, typename RESPONSE>
class	Service: public VirtualServers
{
	public:
		Service() {};
		~Service() {};

		RESPONSE compose(const REQUEST& request, const std::string &port) {
			// getServer(port, request).root
			return (response(request, getServer(port, request)));
		}
};

#endif