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
	TimeOutProcessLessConnections(3); // not too relevent for http only but check connectionless processes
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
		if (conn_it->second.downloadBufferReady() && FindProcess(&(conn_it->second)) == processes.end())
		{
			std::cout << "Inbounded" << std::endl;
			CreateProcess(&(conn_it->second));
		}
		conn_it++;
	}
}

Server::ProcessList::iterator	Server::FindProcess(Connection *connection)
{
	Server::ProcessList::iterator it = processes.begin();

	while (it != processes.end())
	{
		if ((*it)->id(connection))
		{
			// std::cout << "found" << std::endl;
			return (it);
		}
		it++;
	}
	return (it);
}

Server::ProcessList::iterator	Server::FindProcess(ServiceProcess *service)
{
	Server::ProcessList::iterator it = processes.begin();

	while (it != processes.end())
	{
		if (*it == service)
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
			process->setTimeOutDurration(90);
			processes.push_back(process);
			std::cout << "porcess addedd: " << processes.size() << std::endl;
		}
		it++;
	}
}

void	Server::KillProcess(ServiceProcess *process)
{
	closeConnection(&(process->theConnection()));
	delete (process);
	processes.remove(process);
	std::cout << "after kill: " << processes.size() << std::endl;
}

void	Server::EndProcess(ServiceProcess *process)
{
	delete (process);
	processes.remove(process);
	std::cout << "after rm: " << processes.size() << std::endl;
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

#define PROCESSES_LIMIT 60

void Server::CreateProcess(ServiceProcess *process)
{
	// EndProcess(process);
	if (process)
		processes.push_back(process);
	if (processes.size() > PROCESSES_LIMIT / 2)
		closeOlderProcesses(3);
	if (processes.size() > PROCESSES_LIMIT)
		closeOlderProcesses(1, false);
	std::cout << "after create: " << processes.size() << std::endl;
};

void Server::closeOlderProcesses(const size_t &age, bool keepLatest)
{
	ProcessList::iterator	it = processes.begin();

	if (keepLatest)
		it++;
	while (it != processes.end())
	{
		(*it)->setTimeOutDurration(age);
		it++;
	}
}

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

#define	TIMEDOUT_CREDIBLE 20000 //prevent connectionless pulls before closure
#define	CLOSE_CLIENT true //signify connaction for kill

bool	Server::pullIncoming(Connection &connection) {
	if (connection.getElapsedTime() < TIMEDOUT_CREDIBLE)
	{
		try {
			return (connection.receivePacket());
		} catch (...) {
			connection.updateTime(CLOSE_CLIENT);
			return (false);
		}
	}
	return (false);
}

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
		connection.flushIncoming();
		// std::cout << connection;
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