#ifndef SERVER_HPP
# define SERVER_HPP

# include <includes.hpp>

class	Observer
{
	private:
		std::vector<struct pollfd>	vector;
		static const int MAX_CONNECTIONS = 10;
		static const int TIMEOUT = 5000;
	public:
		Observer();
		~Observer() {};

		int	Poll(const bool timeOutOn = false);

	protected:

		void	insertFileDescriptor(const int fd, const int events = POLLIN | POLLOUT, const bool asynch = true);
		void	removeFileDescriptor(const int fd);
		bool	checkFd(const int fd, const int events = POLLIN);

	private:
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
		~PortSockets() {};

		template <typename PARSER>
		void	startPorts(std::list<std::string> (*parsingMethod)(PARSER &), PARSER parser, bool asynch = true);

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
	std::list<std::string>::iterator	it;
	std::list<std::string>				socketList;
	int									fd;

	socketList = parsingMethod(parser);
	it = socketList.begin();
	while (it != socketList.end())
	{
		if (portSockets.find(*it) == portSockets.end())
		{
			fd = this->socketInitMethod(it->c_str());
			portSockets[*it] = fd;
			insertFileDescriptor(fd, POLLIN, asynch);
		}
		it++;
	}
};


# include <Client.hpp>
#define TIMEOUT 10

class	ConnectionQueController: virtual public Observer
{
	public:
		typedef	std::map<int, Client>	listOfClients;
	private:
		listOfClients	Clients;

	public:
		ConnectionQueController() {};
		~ConnectionQueController() {};
	protected:

		void	setClients(std::list<std::pair<std::string, int> > const &loudPortList);
		void	closeTimeOut();

		void	queProcess(bool (*action)(Client &client), const int observer_event = POLLIN);
	
		template <typename TYPE>
		void	action(void (*action)(Client &, TYPE), TYPE insertion);
		void	action(void (*action)(Client &client));

	private:
		void	closeConnection(listOfClients::iterator &position);
};

template <typename TYPE>
void	ConnectionQueController::action(void (*action)(Client &, TYPE), TYPE insertion)
{
	listOfClients::iterator	it = Clients.begin();

	while (it != Clients.end())
	{
		action(it->second, insertion);
		it++;
	}
}

# include <Service.hpp>

template<typename SERVICE = Service*>
class Server : public ConnectionQueController, public PortSockets
{
	public:
		typedef std::list<SERVICE>				ServiceList;
		typedef std::list<void (*)(Client &)>	ActionList;

	private:
		ActionList		actions;
		ServiceList		services;

	public:
		Server() {}
		~Server() {}
		
		void setAction(void (*Action)(Client &)) {
			actions.push_back(Action);
		};

		void setService(SERVICE service) {
			services.push_back(service);
		};

		void Run() {
			Observer::Poll(true);

			setClients(getLoudSockets());
			queProcess(pullIncoming, POLLIN);
			DoActions();
			queProcess(pushOutgoing, POLLOUT);
			closeTimeOut();
		};

	private:
		void DoActions() {
			ActionList::const_iterator it = actions.begin();
			while (it != actions.end()) {
				action(*it);
				it++;
			}

			typename	ServiceList::const_iterator it2 = services.begin();
			while (it2 != services.end()) {
				action(ServiceDefault, *it2);
				it2++;
			}
		};

		void	ServiceHandlers();

		static bool pullIncoming(Client &client);
		static bool pushOutgoing(Client &client);
		static bool validateConnections(Client &client);
		static void default_action(Client &client) {
			(void)client;
		}
		static void	ServiceDefault(Client &client, SERVICE service);
};

template<typename SERVICE>
bool	Server<SERVICE>::pullIncoming(Client &client) {
	return (client.receivePacket());
}

#define	CLOSE_CLIENT true

template<typename SERVICE>
bool	Server<SERVICE>::pushOutgoing(Client &client)
{
	if (client.ready())
	{
		try {
			return (client.sendPacket());
		} catch(const std::exception& e) {
			client.updateTime(CLOSE_CLIENT);
			return (false);
		}	
	}
	return (false);
}

template<typename SERVICE>
void	Server<SERVICE>::ServiceDefault(Client &client, SERVICE service)
{
	if (service->Ready(client)) {
		service->Serve(client);
	}
	service->Handle(client);
}

#endif