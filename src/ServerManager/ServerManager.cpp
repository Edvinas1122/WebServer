#include <ServerClasses.hpp>

void	ServerManager::serverCreator(const char *path)
{
	ConfigurationFile	configurationFile(path);
	DescendParser		parser;

	parser.setFile(configurationFile);
	parseServers(parser);
	startPorts(parser);
}


void	ServerManager::Start()
{
	Poll(true);
	setClients(getLoudSockets());
	readClients();
	closeTimeOut();
}


// WaitForClientRequest()
// BuildRequest() //showes it in
// AssigningVirtualServerToClient()
// BuildingResponse() 
// beSendingResponse() || beReceiving() || close()

