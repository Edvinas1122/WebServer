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
			std::cout << "received client" << std::endl;
		}
	}
}

void	ClientsQue::removeClient(listOfClients::iterator &position)
{
	std::cout << "closeFd: " << position->first << std::endl;
	removeFileDescriptor(position->first);
	Clients.erase(position);
}

void	ClientsQue::readClients()
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		if (checkFd(it->first)) {
			it->second.receivePacket();
			it->second.updateTime();
			std::cout << it->second.getMessage() << std::endl;
			it->second << "Message received";
		}
		it++;
	}
}

void	ClientsQue::respondClients()
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		if (it->second.ready() && checkFd(it->first, POLLOUT))
		{
			it->second.sendPacket();
			it->second.updateTime();
		}
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
			std::cout << "elapsed: " << (*it).second.getElapsedTime() << std::endl;
			removeClient(it);
			it = Clients.begin();
			if (it == Clients.end())
				break;
		}
		it++;
	}
}