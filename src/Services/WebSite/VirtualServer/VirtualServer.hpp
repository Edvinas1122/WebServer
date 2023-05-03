#ifndef VIRTUALSERVE_HPP
# define VIRTUALSERVE_HPP

#include <includes.hpp>
# include <DescendParser.hpp>
/*
	Set a default file to answer if the request is a directory
	Make it work with POST and GET methods
	Make the route able to accept uploaded files and configure where they should
	be saved.
*/

class Route
{
	private:
		bool						directory_listing_enabled;
		std::string					response_dir;
		std::string					upload_dir;
		std::string					redirect;
		std::string					default_file;
		std::map<std::string, bool>	forbit_methdods;
		size_t						max_body_size;
	public:
		Route(): directory_listing_enabled(true), response_dir(""),
				upload_dir(""), redirect(""), default_file(""),
				max_body_size(std::numeric_limits<size_t>::max()) {};
		Route(DescendParser parser);

	void				displayInfo(const char *append) const;
	const std::string	getResponseDir() const {return (response_dir);};
	const std::string	getDefaultFile() const;
	const std::string	getRedirect() const { return (redirect);};
	bool	MethodPermited(std::string const &method) const {
		return (!forbit_methdods.at(method));
	};
	bool	DirListingPermiter() const {return (directory_listing_enabled);};
	size_t	getMaxBodySize() const {return (max_body_size);};
	const std::string	getUploadDir() const {return (upload_dir);};

	private:

	void	ParseAllowedMethods(DescendParser &parser);
	void	ParseForbidMethods(DescendParser &parser);
	void	SetMethodsDefault(); // all forbid except GET
	public:

	class	RouteHasNoDirNorRedirect: public std::exception {};
};

class VirtualServer {
	private:
		std::string							host;
		std::string							server_name;
		std::string							root_dir;
		std::string							index;
		std::string							ssl_certificates;
		std::string							ssl_keys;
		std::map<std::string, std::string>	ssl_ports;
		std::map<std::string, std::string>	cgi_response;
		std::map<std::string, std::string>	error_pages;
		std::list<std::string>				port_number;
		std::map<std::string, Route>		locations;
		size_t								max_body_size;

	public:
		VirtualServer() {};
		VirtualServer(DescendParser parser);

		Route		getLocation(std::string const &location);
		const char	*getHost() const;
		const char	*getServerName() const;
	void		displayInfo() const;

	bool					methodPermited(std::string const &dir, std::string const &method);
	const std::string		getUploadDir(std::string const &dir, std::string const &filename) const;
	bool					dirListingPermited(std::string const &dir);
	const std::string		getSystemPath(std::string const &dir, std::string const &filename) const;
	const std::string		getRedirectMessage(std::string const &dir, std::string const &url);
	bool					isCGI(std::string const &fileExtention);
	std::string				CGIexecutableDir(std::string const &fileExtention);
	VirtualServer			*validatePort(std::string const &port);
	std::list<std::string>	getPorts() {return (port_number);};
	const std::string		errorPage(const unsigned int &error_number);
	size_t					maxRecevieSize(std::string const &dir, std::string const &filename) const;
	const std::string		getDefaulFile(std::string const &dir);
	const Route				*findDescendLevel(std::string const &dir) const;
	const Route				*getRouteCout(std::string const &dir, std::string const &filename) const;
	const std::string		getScheme(std::string const &port);
	std::list<std::pair<SSL_CTX*, std::string> >	getSSLSocketMap() const;	
	private:
		const std::string	getSystemRoot(std::string const &urlDir);
		const Route			*getRoute(std::string const &path, std::string const &filename) const;
	public:
		static bool	validServerName(std::string const &str);
		static bool	validIndexFile(std::string const &str);
		static bool	pathcheck(std::string const &str);
		static bool	isPositiveNumber(std::string const &str);
		static bool isPositiveNumberInErrorRange(std::string const &str);
		static bool validPath(std::string const &str);
		static bool validCGI(std::string const &str);
};


class	VirtualServers {
	public:
		typedef	std::map<std::string, VirtualServer>	virtualServerMap;
	protected:
		virtualServerMap	virtualServers;
	
	public:
		VirtualServers() {};
		~VirtualServers() {};

	void					Info() const;
	void					parseConfigurationFile(const char *path);
	VirtualServer			*getServer(std::string const &port, std::string const &host);
	std::list<std::string>	getPortList();
	std::list<std::pair<SSL_CTX*, std::string> >	getSSLPortList();
	// std::list<std::pair<std::string, <std::pair<std::string, std::string> > > >	getSLLPorts();


	VirtualServer			*getDefault() {return (&virtualServers.begin()->second);};

	public:
		void parseServers(DescendParser &parser)
		{
			int	iterator = 1;

			while (parser.count("server") >= iterator)
			{
				virtualServers[parser.get<VirtualServer>("server", iterator).getHost()] = parser.get<VirtualServer>("server", iterator);
				iterator++;
			};
		};

};

#endif
