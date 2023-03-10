#include <ServerClasses.hpp>
#include <openPortSocket.hpp>

void	ServerManager::serverCreator(const char *path)
{
	ConfigurationFile	configurationFile(path);
	DescendParser		parser;
	int					iterator = 1;

	parser.setFile(configurationFile);
	while (parser.count("server") >= iterator)
	{
		VirtualServers[parser.get<VirtualServer>("server", iterator).getHost()] = parser.get<VirtualServer>("server", iterator);
		iterator++;
	}
	iterator = 1;
	while (parser.count("listen") >= iterator)
	{
		if (portSockets.find(parser.getValue("listen", iterator)) == portSockets.end())
			portSockets[parser.getValue("listen", iterator)] = openPortSocket(parser.getValue("listen", iterator).c_str());
		iterator++;
	}
}

void	ServerManager::PrintServers()
{
	virtualServerMap::iterator	it = VirtualServers.begin();

	while (it != VirtualServers.end())
	{
		it->second.displayInfo();
		it++;
	}
}

void	ServerManager::checkPortSockets() {

	std::map<std::string, int>::const_iterator	socket_it = portSockets.begin();
	int											client = 0;
	struct sockaddr_in							clientaddr;
	static socklen_t							addrlen = sizeof(clientaddr);

	while (socket_it != portSockets.end())
	{
		client = accept(socket_it->second, (struct sockaddr *) &clientaddr, &addrlen);
		if (client > 0) {
			// buildEmptyClient();
			// addClient();
			std::cout << client << std::endl;
		}
		socket_it++;
	}
}

// WaitForClientRequest()
// BuildRequest() //showes it in
// AssigningVirtualServerToClient()
// BuildingResponse() 
// beSendingResponse() || beReceiving() || close()

