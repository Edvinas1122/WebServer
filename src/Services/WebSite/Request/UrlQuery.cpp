#include "HTTP.hpp"

const char* UrlQuery::dir_delim = "/";
const char* UrlQuery::query_delim = "?\0";

const std::string	UrlQuery::getFileName() const
{
	size_t	begin = find_last_of(dir_delim) + 1;
	size_t	end =  find_first_of(query_delim);

	std::string	filename = substr(begin, end - begin);
    std::string::size_type	pos = 0;

    while ((pos = filename.find("%20", pos)) != std::string::npos)
	{
        filename.replace(pos, 3, " ");
        pos += 1;
    }
	if (begin >= end)
		return ("");
	return (filename);
}

const std::string	UrlQuery::getDir() const
{
	size_t	begin = find_first_of(dir_delim) + 1;
	size_t	end =  find_last_of(dir_delim) + 1;

	if (begin >= end)
		return ("");
	return (substr(begin, end - begin));
}

const std::string	UrlQuery::getFileExtension() const
{
	return (getFileName().substr(getFileName().find_first_of(".")));
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