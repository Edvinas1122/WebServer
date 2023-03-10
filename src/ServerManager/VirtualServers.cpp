#include <ServerClasses.hpp>

void	VirtualServers::parseServers(DescendParser &parser)
{
	int	iterator = 1;

	while (parser.count("server") >= iterator)
	{
		virtualServers[parser.get<VirtualServer>("server", iterator).getHost()] = parser.get<VirtualServer>("server", iterator);
		iterator++;
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