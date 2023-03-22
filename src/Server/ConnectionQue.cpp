#include <Server.hpp>
# include <File.hpp>

void	ConnectionQueController::setClients(std::list<std::pair<std::string, int> > const &loudPortList)
{
	struct sockaddr_in				socketAddress;
	socklen_t						addrlen = sizeof(struct sockaddr_in);
	int								clientFd;
	std::list<std::pair<std::string, int> >::const_iterator	it = loudPortList.begin();

	if (loudPortList.size())
	{
		while (it != loudPortList.end())
		{
			clientFd = accept((*it).second, (struct sockaddr *) &socketAddress, &addrlen);
			if (clientFd == -1)
				break;
			if (Clients.find(clientFd) == Clients.end()) {
				Clients.insert(std::pair<int, Client>(clientFd, Client(clientFd, socketAddress, it->first)));
				insertFileDescriptor(clientFd);
			}
			it++;
		}
	}
}

void	ConnectionQueController::closeConnection(listOfClients::iterator &position)
{
	removeFileDescriptor(position->first);
	Clients.erase(position);
}

void	ConnectionQueController::queProcess(bool (*action)(Client &client), const int observer_event)
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		if (checkFd(it->first, observer_event) && action(it->second))
			it->second.updateTime();
		it++;
	}
}

void	ConnectionQueController::action(void (*action)(Client &client))
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		action(it->second);
		it++;
	}
}

void	ConnectionQueController::closeTimeOut()
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		if ((*it).second.getElapsedTime() > (*it).second.getTimeOutDurration())
		{
			closeConnection(it);
			it = Clients.begin();
			if (it == Clients.end())
				break;
		}
		it++;
	}
}