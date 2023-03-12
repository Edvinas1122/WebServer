#include <ServerClasses.hpp>
#include <configurationFileFormat.hpp>

void	ServerManager::serverCreator(const char *path)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(path);
	parser.setContent(configurationFile.GetContents(removeComents));
	VirtualServers::parseServers(parser);
	PortSockets::startPorts(parser);
}

bool	readClients(Client &client)
{
	client.receivePacket();
	return (true);
}


void	printReceived(Client &client)
{
	if (client.getMessage().length()) {
		std::cout << client;
	}
}

#define	CLOSE_CLIENT true

bool	respondClients(Client &client)
{
	if (client.ready())
	{
		try {
			client.sendPacket();
		} catch(const std::exception& e) {
			client.updateTime(CLOSE_CLIENT);
			return (false);
		}	
	}
	return (false);
}

void	sendMessage(Client &client, std::string message)
{
	client << message;
}

void	ServerManager::Start()
{
	Observer::Poll(true);
	ClientsQue::setClients(getLoudSockets());
	ClientsQue::queProcess(readClients, POLLIN);

	ClientsQue::action(printReceived);
	terminal_interface();
	ClientsQue::action(sendMessage, extractMessage());
	clearMessage();

	ClientsQue::queProcess(respondClients, POLLOUT);
	ClientsQue::action()
	ClientsQue::closeTimeOut();
}
