#ifndef SERVER_HPP
# define SERVER_HPP

# include <includes.hpp>

class	Observer
{
	private:
		std::vector<struct pollfd>	vector;
		static const int MAX_CONNECTIONS = 10; // attribute do not limit connections
		static const int TIMEOUT = 5000;
	public:
		Observer();
		~Observer() {vector.clear();};

		/*
			Unix poll https://man7.org/linux/man-pages/man2/poll.2.html
		*/
		int	Poll(const bool timeOutOn = false);

	protected:

		void	insertFileDescriptor(const int fd, const int events = POLLIN | POLLOUT, const bool asynch = true);
		void	removeFileDescriptor(const int fd);
		bool	checkFd(const int fd, const int events = POLLIN);

	private:
		/*
			Unix fcntl https://man7.org/linux/man-pages/man2/fcntl.2.html
		*/
		void	setAsNonBlocking(const int socket_fd) const;

	public:
		class	InvalidFd: public std::exception {};
};


#include "mod/openPortSocket.hpp"

class	PortSockets: virtual public Observer
{
	public:
		typedef std::map<std::string, int>	socketMap;	
	private:
		socketMap	portSockets;
		int	(*socketInitMethod)(char const *);
	public:
		PortSockets() {
			this->socketInitMethod = openPortSocket;
		};
		~PortSockets();

		template <typename PARSER>
		void	startPorts(std::list<std::string> (*parsingMethod)(PARSER &), PARSER parser, bool asynch = true);
		void	startPorts(std::list<std::string> ports, bool asynch = true);
		void	startPort(std::string const &port, bool asynch = true);
	
		void	infoPorts() const;
	protected:
		void	closePorts();
	public:
		void	setStartSocketMethod(int (*socketInitMethod)(char const *)) {
			this->socketInitMethod = socketInitMethod;
		};
	
	protected:
		std::list<int>							getSockets();
		std::list<std::pair<std::string, int> >	getLoudSockets(const int events = POLLIN);
};

template <typename PARSER>
void	PortSockets::startPorts(std::list<std::string> (*parsingMethod)(PARSER &), PARSER parser, bool asynch)
{
	startPorts(parsingMethod(parser), asynch);
};


# include <TCP.hpp>

class	ConnectionQueController: virtual public Observer
{
	public:
		typedef	int									fd_t;
		typedef	std::map<TCPConnectionOrigin, TCP>	listOfConnections;
	protected:
		listOfConnections	Connections;

	public:
		ConnectionQueController() {};
		~ConnectionQueController() {};
	protected:

		void	setConnections(std::list<std::pair<std::string, int> > const &loudPortList);

		void	ProcessQue(bool (*action)(Connection &), const int observer_event = POLLIN);
	
		template <typename T>
		void	action(void (*action)(Connection &, T), T);
		template <typename T>
		void	action(void (*action)(Connection &, T), T, TCPConnectionOrigin);
		void	action(void (*action)(Connection &));

		void	closeConnection(Connection *);
		void	closeConnection(listOfConnections::iterator &position);
		// bool	checkConnection(Connection *);

};

template <typename T>
void	ConnectionQueController::action(void (*action)(Connection &, T), T object)
{
	listOfConnections::iterator	it = Connections.begin();

	while (it != Connections.end())
	{
		action(it->second, object);
		it++;
	}
}

template <typename T>
void	ConnectionQueController::action(void (*action)(Connection &, T), T object, TCPConnectionOrigin conn_id)
{
	Connection connection = Connections.at(conn_id);

	action(connection, object);
}

# include <Service.hpp>
# include <ProgramInterface.hpp>

class Server : public ConnectionQueController, public PortSockets
{
	public:
		typedef std::list<Service*>			ServiceList; // interfaces
		typedef std::list<ServiceProcess*>	ProcessList; //
		// typedef std::map<TCPConnectionOrigin, ServiceProcess*>	ProcessList; //

	private:
		ServiceList		services;
		ProcessList		processes;

	public:
		Server() {};
		~Server();
		
	void	Run();
	void	addService(Service *service);

	private:

	void					TimeOutProcessLessConnections(const int);
	void					StartProcesses();
	void					CreateProcess(Connection *connection, TCPConnectionOrigin const &origin);
	void					CreateProcess(ServiceProcess *process);
	void					CreateProcesses(ProcessList &beginers);
	void					KillProcesses(ProcessList &deads);
	void					KillProcess(ServiceProcess *process);
	void					EndProcesses(ProcessList &finisheds);
	void					EndProcess(ServiceProcess *process);
	void					Serve();
	ProcessList::iterator	FindProcess(Connection *connection);
	ProcessList::iterator	FindProcess(ServiceProcess *service);
	void					closeOlderProcesses(const size_t &age, bool keepLatest = true);

	ConnectionQueController::listOfConnections	getConnectionsByOrigin(in_addr const &ipAddress);
	void	HeartBeatIdleProcessesFromSameOrigin(TCPConnectionOrigin const &, Service*);

	static bool pullIncoming(Connection &client);
	static bool pushOutgoing(Connection &client);
	static void wipeIncomingBuffers(Connection &client);
};

#endif