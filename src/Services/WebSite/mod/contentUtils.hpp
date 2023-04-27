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
std::string			to_lower(std::string const &bigger);
const std::string	fixDir(std::string const &urlDir);
const std::string	setVar(std::string const &key, std::string const &value);

/*
	Cryptic utils
*/

std::string	generateWebSocketAccept(const std::string& key);

/*
	Compression utils
*/
// std::string decompress(const std::string& compressed);

Buffer send_websocket_message(const std::string& message);
std::string	receive_websocket_message(uint8_t const *data, uint64_t const len);

std::string generate_random_string();

#endif