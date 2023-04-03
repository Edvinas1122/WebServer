#include <Server.hpp>
# include <File.hpp>

/*
	TCP connections only...
*/
void	ConnectionQueController::setConnections(std::list<std::pair<std::string, int> > const &loudPortList)
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
			if (Connections.find(clientFd) == Connections.end()) {
				Connections.insert(std::pair<int, TCP>(clientFd, TCP(clientFd, socketAddress, it->first)));
				insertFileDescriptor(clientFd);
			}
			it++;
		}
	}
}

void	ConnectionQueController::closeConnection(Connection *connection)
{
	listOfConnections::iterator	conn_it = Connections.begin();

	while (conn_it != Connections.end())
	{
		if (&(conn_it->second) == connection)
		{
			closeConnection(conn_it);
			break;
		}
		conn_it++;
	}
}

void	ConnectionQueController::closeConnection(listOfConnections::iterator &position)
{
	removeFileDescriptor(position->first);
	Connections.erase(position);
}

// void	ConnectionQueController::closeConnection(fd_t const &fd)
// {
// 	removeFileDescriptor(fd);
// 	Connections.erase(Connections.at(fd));
// }

void	ConnectionQueController::ProcessQue(bool (*action)(Connection &client), const int observer_event)
{
	listOfConnections::iterator	it = Connections.begin();

	while (it != Connections.end())
	{
		if (checkFd(it->first, observer_event) && action(it->second))
			it->second.updateTime();
		it++;
	}
}

void	ConnectionQueController::action(void (*action)(Connection &client))
{
	listOfConnections::iterator	it = Connections.begin();

	while (it != Connections.end())
	{
		action(it->second);
		it++;
	}
}

// bool	ConnectionQueController::checkConnection(Connection *connection)
// {
// 	listOfConnections::iterator	conn_it = Connections.begin();

// 	while (conn_it != Connections.end())
// 	{
// 		if (&(conn_it->second) == connection)
// 			return (true);
// 		conn_it++;
// 	}
// 	return (false);
// }

// void	ConnectionQueController::closeTimeOut()
// {
// 	listOfConnections::iterator	it = Connections.begin();

// 	while (it != Connections.end())
// 	{
// 		if ((*it).second.getElapsedTime() > (*it).second.getTimeOutDurration())
// 		{
// 			closeConnection(it);
// 			it = Connections.begin();
// 			if (it == Connections.end())
// 				break;
// 		}
// 		it++;
// 	}
// }
