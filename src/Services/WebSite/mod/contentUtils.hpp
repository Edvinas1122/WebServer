#ifndef CONTENTUTILS_HPP
# define CONTENTUTILS_HPP

# include <includes.hpp>
# include <Buffer.hpp>
# include <File.hpp>

/*
	HTML markdown content
*/
std::string	dirInfoHTTPFormat(const char *path, std::string const &url, bool displayUpload);

/*
	Access info
*/
bool	isFile(const std::string &path);
bool	Access(const std::string &path);

std::string	updateDirIfFileExists(std::string const &dir);


#endif