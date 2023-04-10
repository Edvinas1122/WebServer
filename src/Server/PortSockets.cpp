#include <Server.hpp>

PortSockets::~PortSockets()
{
	closePorts();
};

std::list<std::pair<std::string, int> >	PortSockets::getLoudSockets(const int events)
{
	socketMap::iterator	it = portSockets.begin();
	std::list<std::pair<std::string, int> >	loudSocketList;

	while (it != portSockets.end())
	{
		if (checkFd(it->second, events))
		{
			// std::cout << "loud socket: " << it->second << std::endl;
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
	try {
		scoket_fd = this->socketInitMethod(port.c_str());
	} catch (...) {
		throw std::exception();
	}
	portSockets[port] = scoket_fd;
	insertFileDescriptor(scoket_fd, POLLIN, asynch);
}

void	PortSockets::infoPorts() const
{
	std::map<std::string, int>::const_iterator	it = portSockets.begin();

	while (it != portSockets.end())
	{
		std::cout << "Open Port: " << it->first << std::endl;
		it++;
	}
}

void	PortSockets::closePorts()
{
	socketMap::iterator	it = portSockets.begin();

	while (it != portSockets.end())
	{
		removeFileDescriptor(it->second);
		it++;
	}
	portSockets.clear();
}