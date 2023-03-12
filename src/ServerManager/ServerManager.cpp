# include <ServerClasses.hpp>
# include <MessageProcessor.hpp>
# include <configurationFileFormat.hpp>

void	ServerManager::serverCreator(const char *path)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(path);
	parser.setContent(configurationFile.GetContents(removeComents));
	VirtualServers::parseServers(parser);
	PortSockets::startPorts(parser);
}

void	ServerManager::Start()
{
	Observer::Poll(true);

	ClientsQue::setClients(getLoudSockets());

	ClientsQue::queProcess(readClients, POLLIN);
	ClientsQue::action(printReceived);
	ClientsQue::action(buildResponse);
	terminal_interface();
	ClientsQue::action(sendMessage, extractMessage());
	clearMessage();

	ClientsQue::queProcess(respondClients, POLLOUT);
	ClientsQue::closeTimeOut();
}


#define	CLOSE_CLIENT true

bool	ServerManager::respondClients(Client &client)
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

bool	ServerManager::readClients(Client &client)
{
	client.receivePacket();
	client.serviceStatus = false;
	// processor.setObject(client.getMessage());
	// std::cout << processor.getWord(1, 1) << std::endl;
	return (true);
}


void	ServerManager::sendMessage(Client &client, std::string message)
{
	client << message;
}


void	ServerManager::printReceived(Client &client)
{
	if (client.getMessage().length()) {
		std::cout << client;
	}
}

void	ServerManager::buildResponse(Client &client)
{
	if (!client.ready() && !client.responseBuilt())
	{
		Response	response;

		response.Build();
		client << response.get();
		client.serviceStatus = true;
	}
}