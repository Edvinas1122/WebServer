#include <ServerClasses.hpp>

void	ClientsQue::setClients(std::list<int> const &loudPortList)
{
	struct sockaddr_in				socketAddress;
	socklen_t						addrlen = sizeof(struct sockaddr_in);
	int								clientFd;
	std::list<int>::const_iterator	it = loudPortList.begin();

	if (loudPortList.size())
	{
		while (it != loudPortList.end())
		{
			clientFd = accept(*(loudPortList.begin()), (struct sockaddr *) &socketAddress, &addrlen);
			if (clientFd == -1)
				break;
			Clients.insert(std::make_pair(clientFd, Client(clientFd, socketAddress)));
			insertFileDescriptor(clientFd);
			it++;
		}
	}
}

void	ClientsQue::removeClient(listOfClients::iterator &position)
{
	removeFileDescriptor(position->first);
	Clients.erase(position);
}

void	ClientsQue::queProcess(bool	(*action)(Client &client), const int observer_event)
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		if (checkFd(it->first, observer_event) && action(it->second))
			it->second.updateTime();
		// else if (observer_event == BYPASS_OBSERVER)
		// 	action(it->second);
		it++;
	}
}

void	ClientsQue::action(void	(*action)(Client &client))
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		action(it->second);
		it++;
	}
}

#define TIMEOUT 10

void	ClientsQue::closeTimeOut()
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		if ((*it).second.getElapsedTime() > TIMEOUT)
		{
			removeClient(it);
			it = Clients.begin();
			if (it == Clients.end())
				break;
		}
		it++;
	}
}