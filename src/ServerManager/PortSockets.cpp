#include <ServerClasses.hpp>

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