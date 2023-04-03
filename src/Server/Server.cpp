#include "Server.hpp"

/*
	Main server Runtime execution loop
*/
void	Server::Run()
{
	Observer::Poll();
	setConnections(getLoudSockets());
	ProcessQue(pullIncoming, POLLIN);
	Serve();
	StartProcesses();
	ProcessQue(pushOutgoing, POLLOUT);
	action(wipeIncomingBuffers);
	// action(closeTimeOut());
};

void	Server::StartProcesses()
{
	listOfConnections::iterator	conn_it = Connections.begin();

	while (conn_it != Connections.end())
	{
		if (FindProcess(&(conn_it->second)) == processes.end())
			CreateProcess(&(conn_it->second));
		conn_it++;
	}
}

Server::ProcessList::iterator	Server::FindProcess(Connection *connection)
{
	Server::ProcessList::iterator it = processes.begin();

	while (it != processes.end())
	{
		if ((*it)->id(connection))
			return (it);
		it++;
	}
	return (it);
}

void	Server::CreateProcess(Connection *connection)
{
	ServiceList::iterator	it = services.begin();
	ServiceProcess			*process;

	while (it != services.end())
	{
		process = (*it)->RequestParse(connection, connection->getMessage());
		if (process != NULL)
		{
			std::cout << "porcess addedd" << std::endl;
			processes.push_back(process);
		}
		it++;
	}
}

void	Server::KillProcess(ServiceProcess *process)
{
	closeConnection(&(process->theConnection()));
	processes.remove(process);
}

void	Server::Serve()
{
	Server::ProcessList::iterator it = processes.begin();

	while (it != processes.end())
	{
		if ((*it)->isTimedOut()) {
			KillProcess(*it);
			it = processes.begin();
		}
		else if (!(*it)->Finished()) {
			if (!(*it)->Handle()) {
				KillProcess(*it);
				it = processes.begin();
			}
		}
		it++;
	}
}

void Server::addService(Service *service) {
	services.push_back(service);
};

/*
	Connection Handling
*/

bool	Server::pullIncoming(Connection &connection) {
	return (connection.receivePacket());
}

#define	CLOSE_CLIENT true

bool	Server::pushOutgoing(Connection &connection)
{
	if (connection.uploadBufferReady())
	{
		try {
			return (connection.sendPacket());
		} catch(const std::exception& e) {
			connection.updateTime(CLOSE_CLIENT);
			return (false);
		}	
	}
	return (false);
}

void	Server::wipeIncomingBuffers(Connection &connection)
{
	if (connection.downloadBufferReady())
		std::cout << connection;
}


Server::~Server()
{
	ProcessList::iterator	it = processes.begin();

	while (it != processes.end())
	{
		delete	*it;
		it++;
	}
	processes.clear();
};