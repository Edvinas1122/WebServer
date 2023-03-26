#include "HTTP.hpp"

const std::string	HttpRequest::getMethod() const
{
	return (substr(0, find_first_of(' ')));
}

const UrlQuery	HttpRequest::getLocation() const
{
	size_t	begin = find_first_of(' ') + 1;
	size_t	len = substr(begin).find_first_of(' ');

	return (substr(begin, len));
}

const std::string	HttpRequest::getProtocolVersion() const
{
	size_t	begin = find(' ', find(' ') + 1) + 1;
	size_t	len = substr(begin).find_first_of('\n');

	return (substr(begin, len));
}

const std::string	HttpRequest::getHost() const
{
	size_t	begin = find("Host: ") + 6;
	size_t	len = substr(begin).find_first_of('\n');

	return (substr(begin, len));
}

const std::string	HttpRequest::getKeepAlive() const
{
	size_t	begin = find("Connection: ") + 12;
	size_t	len = substr(begin).find_first_of('\n');

	return (substr(begin, len));
}

const HttpHeaders	HttpRequest::getHeaders() const
{
	return (substr(find_first_of('\n')));
}

const std::string	HttpRequest::getBoundry() const
{
	return (getHeaders().at("Content-Type").substr(getHeaders().at("Content-Type").find("boundary=") + 9));
}

const std::string	HttpRequest::getFilename() const
{
	return (substr(find("filename=") + 10, substr(find("filename=") + 11).find('"')));
}

const std::string	HttpRequest::getBody() const
{
	std::string	string;

	return (string);
}