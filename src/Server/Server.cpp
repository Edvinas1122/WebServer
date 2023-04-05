#include "Server.hpp"

/*
	Main server Runtime execution loop
*/
void	Server::Run()
{
	Observer::Poll();
	setConnections(getLoudSockets());
	ProcessQue(pullIncoming, POLLIN);
	StartProcesses(); // runs interfaces
	Serve(); // runs processes
	ProcessQue(pushOutgoing, POLLOUT);
	action(wipeIncomingBuffers);
	TimeOutProcessLessConnections(3);
};

void	Server::TimeOutProcessLessConnections(const int allowedInactiveDurration)
{
	listOfConnections::iterator	it = Connections.begin();

	while (it != Connections.end())
	{
		if (it->second.getElapsedTime() > allowedInactiveDurration && FindProcess(&(it->second)) == processes.end())
		{
			closeConnection(it);
			it = Connections.begin();
			if (it == Connections.end())
				break;
		}
		it++;
	}
}

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


/**
 *	Using interfaces assigns free connection to a process 
 */
void	Server::CreateProcess(Connection *connection)
{
	ServiceList::iterator	it = services.begin();
	ServiceProcess			*process;

	while (it != services.end())
	{
		process = (*it)->RequestParse(connection, connection->getMessage());
		if (process != NULL)
		{
			process->setTimeOutDurration(30); 
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

void	Server::EndProcess(ServiceProcess *process)
{
	processes.remove(process);
}

void	Server::Serve() // check for loosening connections - connections with no process
{
	Server::ProcessList::iterator it = processes.begin();
	ProcessList	pullNewProcesses;
	ProcessList	killProcesses;

	while (it != processes.end())
	{
		if ((*it)->isTimedOut()) {
			killProcesses.push_back(*it);
		}
		else if (!(*it)->Finished())
		{
			try {
				if (!(*it)->Handle())
					pullNewProcesses.push_back(*it);
			}
			catch(...) {
				std::cout << "Handle aborted" << std::endl;
				killProcesses.push_back(*it);
			}
		} else
			pullNewProcesses.push_back(*it);
		it++;
	}
	KillProcesses(killProcesses);
	CreateProcesses(pullNewProcesses);
	EndProcesses(pullNewProcesses);
}

void Server::addService(Service *service) {
	services.push_back(service);
};

void	Server::CreateProcesses(ProcessList &beginers)
{
	ProcessList::iterator	create_it = beginers.begin();

	while (create_it != beginers.end())
	{
		CreateProcess((*create_it)->NextProcess());
		create_it++;
	}
}

void	Server::EndProcesses(ProcessList &finisheds)
{
	ProcessList::iterator	finished_it = finisheds.begin();

	while (finished_it != finisheds.end())
	{
		EndProcess(*finished_it);
		finished_it++;
	}
}

void Server::CreateProcess(ServiceProcess *process) {
	if (process)
		processes.push_back(process);
};


void	Server::KillProcesses(ProcessList &deads)
{
	ProcessList::iterator	kill_it = deads.begin();

	while (kill_it != deads.end())
	{
		KillProcess(*kill_it);
		kill_it++;
	}
}

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