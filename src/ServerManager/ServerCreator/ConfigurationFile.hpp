#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

# include <includes.hpp>

/*
	check unrecognized syntax
*/

class ConfigurationFile
{
	private:
		const char	*path;
		std::string	content;
	public:
		ConfigurationFile(const char *config_file_path): path(config_file_path) {};
		std::string	getContent();

	private:
		void	accessCheck() EXCEPTION;
		void	loadFile() EXCEPTION;
		void	removeComents();

	public:
		class	ConfFileAccessFailure: public std::exception {};
};

#endif