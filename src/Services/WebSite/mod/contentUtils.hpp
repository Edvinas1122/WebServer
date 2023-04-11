#ifndef CONTENTUTILS_HPP
# define CONTENTUTILS_HPP

# include <includes.hpp>
# include <Buffer.hpp>
# include <File.hpp>


/*
	HTTP Response headers
*/
// std::string	HTTPHeaderFileOK(std::string const &path);
// std::string	HTTPHeaderDirOK(std::string const &path, const std::string &url);


/*
	HTML markdown content
*/
std::string	dirInfoHTTPFormat(const char *path, std::string const &url, bool displayUpload);
// std::string	typeInfo(std::string const &path);
// std::string	UploadForm(std::string const &path);
// std::string	DeleteForm(std::string const &path);

/*
	Access info
*/
bool	isFile(const std::string &path);
bool	Access(const std::string &path);

/*
	Trim buffers
*/
void	trimUntilFileBegin(Buffer &buffer);
Buffer	unchunkBegining(Buffer buffer, std::string const &delimeter);

#endif