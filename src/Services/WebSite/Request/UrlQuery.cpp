#include "HTTP.hpp"

const char* UrlQuery::dir_delim = "/";
const char* UrlQuery::query_delim = "?\0\r";

static const std::string	removeHexSpaces(std::string const &string)
{
	std::string				filename = string;
	std::string::size_type	pos = 0;

	while ((pos = filename.find("%20", pos)) != std::string::npos)
	{
		filename.replace(pos, 3, " ");
		pos += 1;
	}
	return (filename);
}

const std::string	UrlQuery::getFileName() const
{
	size_t	dotPoss = find_last_of(".");
	size_t	begin = substr(0, dotPoss).find_last_of(dir_delim) + 1;
	size_t	end =  find_first_of(std::string(query_delim) + dir_delim, dotPoss);

	if (begin >= end)
		return ("");
	return (removeHexSpaces(substr(begin, end - begin)));
}

const std::string	UrlQuery::getDir() const
{
	size_t	begin = find_first_of(dir_delim) + 1;
	size_t	end =  find_last_of(dir_delim) + 1;

	if (begin >= end)
		return ("");
	return (removeHexSpaces(substr(begin, end - begin)));
}

const std::string	UrlQuery::getFileExtension() const
{
	try {
		return (getFileName().substr(getFileName().find_first_of(".")));
	} catch (...) {
		return ("");
	}
}

const std::string	UrlQuery::getCGIPathInfo() const
{
	size_t	begin = find(getFileExtension()) + getFileExtension().length();
	size_t	end = find_last_of(query_delim);

	if (begin >= end)
		return ("");
	return (removeHexSpaces(substr(begin, end - begin)));
}

const std::string	UrlQuery::getQuery() const
{
	size_t	begin = find_last_of(query_delim);
	size_t	end = length();

	if (begin >= end)
		return ("");
	return (substr(begin, end - begin));
}

const std::string	UrlQuery::getPath() const
{
	return (getDir() + getFileName());
}