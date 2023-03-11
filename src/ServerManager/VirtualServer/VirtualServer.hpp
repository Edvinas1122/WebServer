#ifndef VIRTUALSERVE_HPP
# define VIRTUALSERVE_HPP

#include <includes.hpp>

/*
	Set a default file to answer if the request is a directory
	Make it work with POST and GET methods
	Make the route able to accept uploaded files and configure where they should
	be saved.
*/

class Route
{
	private:
		bool		directory_listing_enabled;
		std::string	response_dir;
		std::string	upload_dir;
		std::string	redirect;
		std::string	default_file;
		short		forbit_methods[8];
	public:
		Route() {};
		Route(DescendParser parser);

		void	displayInfo(const char *append) const;
};

/*
	Response types:
		1 string -	text or html or redirect
		2 fd -		file or cgi
		or 
		1 fd - 		
*/

/*
	Limit client body size
	server_names can be missing or not

	Max body size implament

	CGI ?
	atribute validators - design- a bit more completed validation

*/

class VirtualServer {
	private:
		std::string							host;
		std::string							server_name;
		std::string							root_dir;
		std::string							index;
		std::map<std::string, std::string>	cgi_response;
		std::map<std::string, std::string>	error_pages;
		std::list<std::string>				port_number;
		std::map<std::string, Route>		locations;
		size_t								max_body_size;
		// std::map<std::string, CGI>			cgi_map;
	public:
		VirtualServer() {};
		VirtualServer(DescendParser parser);
		// VirtualServer(const VirtualServer &src);
		// VirtualServer& operator=(const VirtualServer& other);
		const char	*getRoot() const;
		Route		getLocation(std::string const &location); // <--- /jumper/tall/ > "null or /jumper/"
		const char	*getHost() const;
		const char	*getServerName() const;
		void		displayInfo() const;

	public:
		static bool	validServerName(std::string const &str);
		static bool	validIndexFile(std::string const &str);
		static bool	pathcheck(std::string const &str);
		static bool	isPositiveNumber(std::string const &str);
};

#endif
