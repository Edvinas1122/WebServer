#include "VirtualServer.hpp"

template <typename container>
static void	copy_list_values(container &destination, container const &source)
{
	std::copy(source.begin(), source.end(), std::back_inserter(destination));
}

VirtualServer::VirtualServer(DescendParser parser)
{
	int	iterator = 1;

	port_number = parser.getValuesList("listen", this->isPositiveNumber);
	root_dir = parser.getValue("root", 1, this->pathcheck);
	try {
		index = parser.getValue("index");
	} catch (DescendParser::NoKeyExcept &e) {
		index = "";
	}
	try {
		server_name = parser.getValue("server_name");
	} catch (DescendParser::NoKeyExcept &e) {
		server_name = "Serv1122";
	}
	try {
		host = parser.getValue("host");
	} catch (DescendParser::NoKeyExcept &e) {
		host = "default";
	}
	try {
		max_body_size = atol(parser.getValue("max_body_size").c_str());
	} catch (DescendParser::NoKeyExcept &e) {
		max_body_size = 2000000; //default 2 mb
	}
	while (parser.count("location") >= iterator)
	{
		try {
			locations.insert(parser.getMaped<Route>("location", iterator));
		} catch (Route::RouteHasNoDirNorRedirect &e) {
			std::cerr << "Route has no systemRoute nor Redirect" << std::endl;
			throw std::exception();
		}
		iterator++;
	}
	iterator = 1;
	while (parser.count("status") >= iterator)
	{
		error_pages.insert(parser.getPair("status", iterator));
		iterator++;
	}
	iterator = 1;
	while (parser.count("use_cgi") >= iterator)
	{
		cgi_response.insert(parser.getPair("use_cgi", iterator));
		iterator++;
	}
}

const char	*VirtualServer::getHost() const
{
	return (host.c_str());
}

const char	*VirtualServer::getServerName() const
{
	return (server_name.c_str());
}

void	VirtualServer::displayInfo() const
{

	std::cout << "===" << std::endl;
	std::cout << "Server Name: " << server_name << std::endl;
	std::cout << "Host: " << host << std::endl;
	std::cout << "Root Dir: " << root_dir << std::endl;
	std::cout << "Index: " << index << std::endl;

	std::list<std::string>::const_iterator	str_iterator = port_number.begin();
	if (str_iterator != port_number.end())
		std::cout << "Sockets: " << std::endl;
	while (str_iterator != port_number.end())
	{
		std::cout << "  port: " << *str_iterator << std::endl;
		str_iterator++;
	}
	
	std::map<std::string, std::string>::const_iterator	iterator_2 = cgi_response.begin();
	if (iterator_2 != cgi_response.end())
		std::cout << "CGI:" << std::endl;
	while (iterator_2 != cgi_response.end())
	{
		std::cout << "  extension " << iterator_2->first << " to: " << iterator_2->second << std::endl;
		iterator_2++;
	}

	iterator_2 = error_pages.begin();
	if (iterator_2 != error_pages.end())
		std::cout << "Error Responses:" << std::endl;
	while (iterator_2 != error_pages.end())
	{
		std::cout << "  error " << iterator_2->first << " page: " << iterator_2->second << std::endl;
		iterator_2++;
	}

	std::map<std::string, Route>::const_iterator	iterator = locations.begin();
	if (iterator != locations.end())
		std::cout << "Total locations: " << locations.size() << std::endl;
	while (iterator != locations.end())
	{
		iterator->second.displayInfo("   ");
		iterator++;
	}
	std::cout << "===" << std::endl;
}



#include <File.hpp>
#include <configurationFileFormat.hpp>

void	VirtualServers::parseConfigurationFile(const char *path)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(path);
	parser.setContent(configurationFile.GetContents(removeComents));
	try {
		parseServers(parser);
	} catch (...)
	{
		std::cerr << "Configuration file failure" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	VirtualServers::Info() const
{
	virtualServerMap::const_iterator	it = virtualServers.begin();

	while (it != virtualServers.end())
	{
		it->second.displayInfo();
		it++;
	}
}

VirtualServer	*VirtualServers::getServer(std::string const &port, std::string const &host)
{
	(void) host;
	(void) port;
	return (&virtualServers.at("default"));
}

std::list<std::string>	VirtualServers::getPortList()
{
	std::list<std::string>		portList;
	virtualServerMap::iterator	it = virtualServers.begin(); 

	while (it != virtualServers.end())
	{
		std::list<std::string> vsPortList = it->second.getPorts();
		std::list<std::string>::iterator	vs_it = vsPortList.begin();
		while (vs_it != vsPortList.end())
		{
			if (std::find(portList.begin(), portList.end(), *vs_it) == portList.end())
				portList.push_back(*vs_it);
			vs_it++;
		}
		it++;
	}
	return (portList);
}

static std::string	dirDescend(std::string const &dir, int level)
{
	if (level)
		return (dirDescend(dir.substr(dir.find("/", 1)), level - 1));
	return (dir.substr(0, dir.find("/", 1) + 1));
}

static std::string	dirTrim(std::string const &dir, int level)
{
	if (level)
		return (dirTrim(dir.substr(dir.find("/", 1)), level - 1));
	return (dir);
}

const std::string	fixDir(std::string const &urlDir)
{
	std::string	dirFixed;

	if (urlDir.empty())
		dirFixed = "/";
	else if (urlDir.at(urlDir.length() - 1) != '/')
		dirFixed = std::string("/") + urlDir + "/"; // not recognized by http parser as urldir
	else
		dirFixed = std::string("/") + urlDir;
	return (dirFixed);
}

const std::string	VirtualServer::getSystemRoot(std::string const &urlDir) // double slash
{
	std::string	singleDir = dirDescend(fixDir(urlDir), 0);

	if (locations.find(singleDir) == locations.end())
		return (root_dir + fixDir(urlDir));
	else if (!locations.find(singleDir)->second.getResponseDir().empty())
		return (locations.find(singleDir)->second.getResponseDir() + dirTrim(fixDir(urlDir), 1));
	return ("");
}

const std::string	Route::getDefaultFile()
{
	if (!default_file.empty())
		return (default_file);
	return ("");
};

#include "../mod/contentUtils.hpp"

typedef	struct readDir_s
{
	std::string	realDir;
	std::string	realFileName;
} info_Path_t;


static info_Path_t	setRealPath(std::string const &testDir, std::string const &testFileName, std::string const &sysPath, size_t proxyLevel)
{
	info_Path_t	data;

	if (isDir(sysPath + dirTrim(fixDir(testDir + testFileName), proxyLevel)) && Access(sysPath + dirTrim(fixDir(testDir + testFileName), proxyLevel)))
	{
		data.realFileName = "";
		data.realDir = sysPath + dirTrim(fixDir(testDir + testFileName), proxyLevel);
		return (data);
	}
	data.realFileName = testFileName;
	data.realDir = sysPath + dirTrim(fixDir(testDir), proxyLevel);
	return (data);
}

const std::string	VirtualServer::getSystemPath(std::string const &dir, std::string const &filename)
{
	Route		*route;
	info_Path_t	systemPath;

	if (locations.find(dirDescend(fixDir(dir), 0)) != locations.end()) { // dir is dir, filename is filename
		route = &locations.find(dirDescend(fixDir(dir), 0))->second;
	} else if (locations.find(dirDescend(fixDir(dir + filename), 0)) != locations.end()) { // filename is route
		route = &locations.find(dirDescend(fixDir(dir + filename), 0))->second;
	} else { // no Route
		systemPath = setRealPath(dir, filename, root_dir, 0);
		if (systemPath.realFileName.empty())
			systemPath.realFileName += index;
		return (systemPath.realDir + systemPath.realFileName);
	}
	if (!route->getRedirect().empty()) // redirect
		return ("");
	systemPath = setRealPath(dir, filename, route->getResponseDir(), 1);
	if (systemPath.realFileName.empty())
		systemPath.realFileName += route->getDefaultFile();
	return (systemPath.realDir + systemPath.realFileName);
}

const std::string	VirtualServer::getRedirectMessage(std::string const &dir)
{
	std::string	redirectMessage;

	redirectMessage = "HTTP/1.1 302 Found\r\n";
	redirectMessage += "Location: ";
	redirectMessage += locations.find(dirDescend(fixDir(dir), 0))->second.getRedirect();
	redirectMessage += "\r\n\r\n";
	return (redirectMessage);
}

bool	VirtualServer::isCGI(std::string const &fileExtention)
{
	if (cgi_response.find(fileExtention) != cgi_response.end())
		return (true);
	return (false);
}

std::string	VirtualServer::CGIexecutableDir(std::string const &fileExtention)
{
	return (cgi_response.find(fileExtention)->second);
}

bool	VirtualServer::methodPermited(std::string const &dir, std::string const &method)
{
	if (locations.find(dirDescend(fixDir(dir), 0)) == locations.end())
	{
		if (method == "GET")
			return (true); // root dir methods
		return (false);
	}
	Route		*route = &locations.find(dirDescend(fixDir(dir), 0))->second;
	return (route->MethodPermited(method));
}

bool	VirtualServer::dirListingPermited(std::string const &dir)
{
	if (locations.find(dirDescend(fixDir(dir), 0)) == locations.end())
		return (false);
	Route		*route = &locations.find(dirDescend(fixDir(dir), 0))->second;
	return (route->DirListingPermiter());
}

const std::string	VirtualServer::errorPage(const unsigned int &error_number)
{
	if (ErrorResponsePages.find(error_number) != ErrorResponsePages.end())
		return (ErrorResponsePages.at(error_number));
	return ("");
}

