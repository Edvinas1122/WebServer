#include <ServerClasses.hpp>
#include <openPortSocket.hpp>

void	PortSockets::startPorts(DescendParser &parser, bool asynch)
{
	int	fd;
	int	iterator = 1;

	while (parser.count("listen") >= iterator)
	{
		if (portSockets.find(parser.getValue("listen", iterator)) == portSockets.end())
		{
			fd = openPortSocket(parser.getValue("listen", iterator).c_str());
			portSockets[parser.getValue("listen", iterator)] = fd;
			if (asynch)
				insertFileDescriptor(fd);
		}
		iterator++;
	}
}

std::list<int>	PortSockets::getLoudSockets(const int events)
{
	std::map<std::string, int>::iterator	it = portSockets.begin();
	std::list<int>							loudSocketList;

	while (it != portSockets.end())
	{
		if (checkFd(it->second, events))
		{
			std::cout << "loud socket: " << it->second << std::endl;
			loudSocketList.push_back(it->second);
		}
		it++;
	}
	return (loudSocketList);
}

std::list<int>	PortSockets::getSockets()
{
	std::map<std::string, int>::iterator	it = portSockets.begin();
	std::list<int>							socketList;

	while (it != portSockets.end())
	{
		socketList.push_back(it->second);
		it++;
	}
	return (socketList);
}