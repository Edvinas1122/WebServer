#include <VirtualServer.hpp>

template <typename container>
static void	copy_list_values(container &destination, container const &source)
{
	std::copy(source.begin(), source.end(), std::back_inserter(destination));
}

// VirtualServer::VirtualServer(DescendParser parser)
// {
// 	int	iterator = 1;

// 	port_number = parser.getValuesList("listen", this->isPositiveNumber);
// 	root_dir = parser.getValue("root", 1, this->pathcheck);
// 	index = parser.getValue("index");
// 	server_name = parser.getValue("server_name");
// 	host = parser.getValue("host");
// 	while (parser.count("location") >= iterator)
// 	{
// 		locations.insert(parser.getMaped<Route>("location", iterator));
// 		iterator++;
// 	}
// 	iterator = 1;
// 	while (parser.count("status") >= iterator)
// 	{
// 		error_pages.insert(parser.getPair("status", iterator));
// 		iterator++;
// 	}
// 	iterator = 1;
// 	while (parser.count("use_cgi") >= iterator)
// 	{
// 		cgi_response.insert(parser.getPair("use_cgi", iterator));
// 		iterator++;
// 	}
// }

const char	*VirtualServer::getHost() const
{
	return (host.c_str());
}

const char	*VirtualServer::getServerName() const
{
	return (server_name.c_str());
}

const char	*VirtualServer::getRoot() const
{
	return (root_dir.c_str());
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