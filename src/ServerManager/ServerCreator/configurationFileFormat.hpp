#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

# include <includes.hpp>

/*
	Server Configuration File specific syntax validation
	and other related validation
*/
	std::string	removeComents(std::string const &content);

	class	ConfFileAccessFailure: public std::exception {};

#endif