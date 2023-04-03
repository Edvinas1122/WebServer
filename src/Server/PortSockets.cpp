#include <Server.hpp>

std::list<std::pair<std::string, int> >	PortSockets::getLoudSockets(const int events)
{
	std::map<std::string, int>::iterator	it = portSockets.begin();
	std::list<std::pair<std::string, int> >	loudSocketList;

	while (it != portSockets.end())
	{
		if (checkFd(it->second, events))
		{
			std::cout << "loud socket: " << it->second << std::endl;
			loudSocketList.push_back(std::make_pair<std::string, int>(it->first, it->second));
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

void	PortSockets::startPorts(std::list<std::string> ports, bool asynch)
{
	std::list<std::string>::iterator	it = ports.begin();

	while (it != ports.end())
	{
		startPort(*it, asynch);
		it++;
	}
}

void	PortSockets::startPort(std::string const &port, bool asynch)
{
	int	scoket_fd;

	if (portSockets.find(port) != portSockets.end())
		return ;
	scoket_fd = this->socketInitMethod(port.c_str());
	portSockets[port] = scoket_fd;
	insertFileDescriptor(scoket_fd, POLLIN, asynch);
}