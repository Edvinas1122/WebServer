#ifndef CONTENTUTILS_HPP
# define CONTENTUTILS_HPP

# include <includes.hpp>
# include <Buffer.hpp>
# include <File.hpp>

/*
	Protocol related
*/
const std::string		getHttpExplanation(const unsigned int code);
const std::string		headerMessage(const int &method_version, const int &code, const size_t content_len = std::numeric_limits<size_t>::max(), bool closeHeader = true);

/*
	HTML markdown content
*/
std::string	dirInfoHTTPFormat(const char *path, std::string const &url, bool displayUpload);

/*
	Access info
*/
bool	isFile(const std::string &path);
bool	isDir(const std::string &path);
bool	Access(const std::string &path);

/*
	Access related
*/
std::string	updateDirIfFileExists(std::string const &dir);

/*
	String manipulation
*/
std::string	to_string(size_t const num);
const std::string	fixDir(std::string const &urlDir);

#endif