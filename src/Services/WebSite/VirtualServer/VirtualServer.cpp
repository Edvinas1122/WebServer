#include "VirtualServer.hpp"

template <typename container>
static void	copy_list_values(container &destination, container const &source)
{
	std::copy(source.begin(), source.end(), std::back_inserter(destination));
}


VirtualServer::VirtualServer(DescendParser parser)
{
	int		iterator = 1;
	bool	hasPorts = true;

	try {
		port_number = parser.getValuesList("listen", this->isPositiveNumber);
	} catch (DescendParser::NoKeyExcept &e)
	{
		hasPorts = false;
	}

	root_dir = parser.getValue("root", 1, validPath);
	try {
		index = parser.getValue("index");
	} catch (DescendParser::NoKeyExcept &e) {
		index = "";
	}
	try {
		server_name = parser.getValue("server_name", 1, validServerName);
	} catch (DescendParser::NoKeyExcept &e) {
		server_name = "Serv1122";
	}
	try {
		host = parser.getValue("host", 1);
	} catch (DescendParser::NoKeyExcept &e) {
		host = "default";
	}
	try {
		max_body_size = atol(parser.getValue("max_body_size", 1, isPositiveNumber).c_str());
	} catch (DescendParser::NoKeyExcept &e) {
		max_body_size = std::numeric_limits<size_t>::max(); //default no limit
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
	while (parser.count("use_cgi") >= iterator)
	{
		cgi_response.insert(parser.getPair("use_cgi", iterator, validPath, validCGI));
		iterator++;
	}
	iterator = 1;
	while (parser.count("error_page") >= iterator)
	{
		error_pages.insert(parser.getPair("error_page", iterator, validPath, isPositiveNumberInErrorRange));
		iterator++;
	}

	iterator = 1;
	while (parser.count("ssl_port") >= iterator)
	{
		ssl_ports.insert(parser.getPair("ssl_port", iterator)); // check valid cert path
		iterator++;
	}
	if (iterator == 1 && !hasPorts)
	{
		std::cerr << "Server definition does not have any ports" << std::endl;
		throw std::exception();
	}
	iterator = 1;
	while (parser.count("ssl_cert") >= iterator)
	{
		ssl_certificates.insert(parser.getPair("ssl_cert", iterator)); // check valid var name
		iterator++;
	}

	iterator = 1;
	while (parser.count("ssl_key") >= iterator)
	{
		ssl_keys.insert(parser.getPair("ssl_key", iterator)); // check valid var name
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

VirtualServer	*VirtualServer::validatePort(std::string const &port)
{
	std::list<std::string>::iterator	it = port_number.begin();

	while (it != port_number.end())
	{
		if (*it == port)
			return (this);
		it++;
	}

	if (ssl_ports.find(port) != ssl_ports.end())
		return (this);
	return (NULL);
};

static std::string	dirDescend(std::string const &dir, int level)
{
	if (level)
		return (dirDescend(dir.substr(0, dir.find("/", 1)), level - 1));
	return (dir.substr(0, dir.find("/", 1) + 1));
}

static std::string	dirTrim(std::string const &dir, int level)
{
	if (level)
		return (dirTrim(dir.substr(dir.find("/", 1)), level - 1));
	return (dir);
}

#include "../mod/contentUtils.hpp"

const std::string	VirtualServer::getSystemRoot(std::string const &urlDir) // double slash
{
	std::string	singleDir = dirDescend(fixDir(urlDir), 0);

	if (locations.find(singleDir) == locations.end())
		return (root_dir + fixDir(urlDir));
	else if (!locations.find(singleDir)->second.getResponseDir().empty())
		return (locations.find(singleDir)->second.getResponseDir() + dirTrim(fixDir(urlDir), 1));
	return ("");
}

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


std::string subtractDirsFromPath(std::string path, int numDirs)
{
	size_t lastSeparator = path.find_last_of("/");
	
	for (int i = 0; i < numDirs; i++)
	{
		if (lastSeparator == std::string::npos || lastSeparator == 0) {
			break;
		}
		lastSeparator = path.find_last_of("/\\", lastSeparator - 1);
	}
	if (lastSeparator == std::string::npos) {
		return "";
	}
	return (path.substr(0, lastSeparator));
}

const Route	*VirtualServer::getRouteCout(std::string const &dir, std::string const &filename) const
{
	size_t	iterator = 0;

	while (!subtractDirsFromPath(fixDir(dir), iterator).empty())
	{
		if (locations.find(fixDir(subtractDirsFromPath(fixDir(dir), iterator))) != locations.end())
			return (&locations.find(fixDir((subtractDirsFromPath(fixDir(dir), iterator))))->second);
		iterator++;
	}
	iterator = 0;
	while (!subtractDirsFromPath(fixDir(dir + filename), iterator).empty())
	{
		if (locations.find(fixDir(subtractDirsFromPath(fixDir(dir + filename), iterator))) != locations.end())
			return (&locations.find(fixDir(subtractDirsFromPath(fixDir(dir + filename), iterator)))->second);
		iterator++;
	}
	return (NULL);
}

const Route	*VirtualServer::getRoute(std::string const &dir, std::string const &filename) const
{
	if (locations.find(dirDescend(fixDir(dir), 0)) != locations.end()) // dir is dir, filename is filename
		return (&locations.find(dirDescend(fixDir(dir), 0))->second);
	else if (locations.find(dirDescend(fixDir(dir + filename), 0)) != locations.end()) // filename is route
		return (&locations.find(dirDescend(fixDir(dir + filename), 0))->second);
	else
		return (NULL);

}

const std::string	VirtualServer::getSystemPath(std::string const &dir, std::string const &filename) const
{
	const Route	*route;
	info_Path_t	systemPath;

	route = getRouteCout(dir, filename);
	if (route == NULL)  {
		systemPath = setRealPath(dir, filename, root_dir, 0);
		if (systemPath.realFileName.empty() && systemPath.realDir == fixDir(root_dir))
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

const std::string	VirtualServer::getRedirectMessage(std::string const &dir, std::string const &url)
{
	std::string	redirectMessage;
	const Route	*route = getRouteCout(dir, "");

	if (!route)
		throw std::exception();
	redirectMessage = "HTTP/1.1 302 Found\r\n";
	redirectMessage += "Location: ";
	redirectMessage += route->getRedirect();
	// redirectMessage += locations.find(dirDescend(fixDir(dir), 0))->second.getRedirect();
	redirectMessage += dirTrim(url, 1);
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
	if (error_pages.find(to_string(error_number)) != error_pages.end())
		return (error_pages.at(to_string(error_number)));
	return ("");
}

size_t	VirtualServer::maxRecevieSize(std::string const &dir, std::string const &filename) const
{
	const Route	*route = getRoute(dir, filename);

	if (!route)
		return (max_body_size);
	return (route->getMaxBodySize());
};

const std::string	VirtualServer::getUploadDir(std::string const &dir, std::string const &filename) const
{
	const Route	*route = getRoute(dir, filename);

	if (!route)
		return ("");
	return (route->getUploadDir());
}

const std::string	VirtualServer::getScheme(std::string const &port)
{
	if (ssl_ports.find(port) != ssl_ports.end())
		return ("https://");
	else
		return ("http://");
}