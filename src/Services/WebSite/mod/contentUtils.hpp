#ifndef CONTENTUTILS_HPP
# define CONTENTUTILS_HPP

# include <includes.hpp>
# include <Buffer.hpp>
# include <File.hpp>

/*
	HTML markdown content
*/

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
std::string			to_string(size_t const num);
const std::string	fixDir(std::string const &urlDir);
const std::string	setVar(std::string const &key, std::string const &value);

#endif