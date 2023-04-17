#include "Server.hpp"

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
	if (outputInbound)
		action(printIncoming);
	action(wipeIncomingBuffers);
	TimeOutProcessLessConnections(3);
	// idleProcessCount();
};

static void	dupToFile(std::string const &path)
{
	int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
}

#include <sys/prctl.h>
static void	startBackground()
{
	int	pid = fork();

	if (pid != 0)
	{
		exit(EXIT_SUCCESS);
	}
	prctl(PR_SET_NAME, "BackGround Server Process", 0, 0, 0);
	if (setsid() < 0) {
		perror("setsid failed");
		exit(1);
	}
}

void	Server::CommandParse(std::string const &data)
{
	if (data.empty())
		return;
	if (data.find("print") != std::string::npos)
		outputInbound = outputInbound ? false : true;
	if (data.find("file_pr") != std::string::npos)
		dupToFile("/home/WebServer/serverLog.txt");
	if (data.find("runtime") != std::string::npos)
		printRunTimeInfo = printRunTimeInfo ? false : true;
	if (data.find("background") != std::string::npos)
		startBackground();
}

/*
	FindProcess ineficient loop
	can be substituted with adding all connections to processes so there would not be processless connections

*/
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
			if (printRunTimeInfo)
				std::cout << "Inbounded" << std::endl;
			CreateProcess(&(conn_it->second), conn_it->first);
		}
		conn_it++;
	}
}

/*
	Heart beat idle proccesses from same origin -> ip address
*/

#define	HEART_BEAT_SAFETY_TIME_MS 210 // single origin multiple connections at same time is better to substitute with type if connection were added via same list

void	Server::HeartBeatIdleProcessesFromSameOrigin(TCPConnectionOrigin const &origin, Service *service)
{
	listOfConnections	matchingOrigin = getConnectionsByOrigin(origin.ipAddress);
	ProcessList::iterator	it = processes.begin();

	while (it != processes.end())
	{
		if ((*it)->ageTimedOut(HEART_BEAT_SAFETY_TIME_MS) && service->DetermineIfIdleProcessType(*it)) {
			listOfConnections::iterator	it_origin = matchingOrigin.begin();

			while (it_origin != matchingOrigin.end())
			{
				if (!(*it)->theConnection().downloadBufferReady() && (*it)->theConnection() == (*it_origin).second) {
					(*it)->HeartBeat();
					if (printRunTimeInfo)
						std::cout << "idle process from same origin heart beat set" << std::endl;
					break;
				}
				it_origin++;
			}
		}
		it++;
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
 *	If process is created from a Connection we should hearbeat IDLE processes that are
 *	from a same origin as a connection
 */
void	Server::CreateProcess(Connection *connection, TCPConnectionOrigin const &origin)
{
	ServiceList::iterator	it = services.begin();
	ServiceProcess			*process;

	while (it != services.end())
	{
		try {
			process = (*it)->RequestParse(connection, connection->getMessage());
		} catch (...) {
			connection->updateTime(CLOSE_CLIENT);
		}
		if (process != NULL)
		{
			(void)origin;
			HeartBeatIdleProcessesFromSameOrigin(origin, *it);
			process->setTimeOutDurration((*it)->TimeOutAge());
			processes.push_back(process);
			if (printRunTimeInfo)
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
	if (printRunTimeInfo)
		std::cout << "after kill: " << processes.size() << std::endl;
}

void	Server::EndProcess(ServiceProcess *process)
{
	delete (process);
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
				{
					(*it)->handleFollowUp();
					pullNewProcesses.push_back(*it);
				}
			}
			catch(...) {
				killProcesses.push_back(*it);
			}
		} else
		{
			(*it)->handleFollowUp();
			pullNewProcesses.push_back(*it);
		}
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
};


/*
	It is not possible trougth system call to determine weather client had closed
	connection or he idles

	There are Methods to determine if a client closed a connection
	1. timeout durration strickness based on incoming rate
	2. heart beat 200 OK message to processless idle keep-alive connections  
*/
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

ConnectionQueController::listOfConnections	Server::getConnectionsByOrigin(in_addr const &ipAddress)
{
	listOfConnections			mathcingOrigin;

#ifndef C98
	return (std::copy_if(Connections.begin(), Connections.end(),
				std::inserter(mathcingOrigin, mathcingOrigin.end()), findByAddr(ipAddress)));
#else
	listOfConnections::iterator	it = Connections.begin();

	while (it != Connections.end())
	{
		if (it->first.ipAddress.s_addr == ipAddress.s_addr)
			mathcingOrigin[it->first] = it->second;
		it++;
	}
	return (mathcingOrigin);
#endif
}

/*
	Connection Handling
*/

#define	TIMEDOUT_CREDIBLE 20000 //prevent connectionless pulls before closure

bool	Server::pullIncoming(Connection &connection)
{
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
}

void	Server::printIncoming(Connection &connection)
{
	if (connection.downloadBufferReady())
		std::cout << connection << std::endl;
}
