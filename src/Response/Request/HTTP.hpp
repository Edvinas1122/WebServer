#ifndef HTTP_MESSAGES_HPP
# define HTTP_MESSAGES_HPP

# include <includes.hpp>

class	UrlQuery: public std::string
{
	private:
		static const char *dir_delim;
		static const char *query_delim;
	public:
		UrlQuery() {};
		UrlQuery(std::string const &fullUrl): std::string(fullUrl) {};

		const std::string	getDir() const;
		const std::string	getFileName() const;
		const std::string	getFileExtension() const;
		const std::string	getQuery() const;
};

class	HttpRequest: public std::string
{
	public:
		HttpRequest() {};
		HttpRequest(std::string const &httpMessage): std::string(httpMessage) {};

		const UrlQuery		getLocation() const;
		const std::string	getMethod() const;
		const std::string	getHost() const;
		const std::string	getKeepAlive() const;
		const std::string	getProtocolVersion() const;
};

#endif