#include <ServerClasses.hpp>
#include <configurationFileFormat.hpp>

void	ServerManager::serverCreator(const char *path)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(path);
	parser.setContent(configurationFile.GetContents(removeComents));
	parseServers(parser);
	startPorts(parser);
}


void	ServerManager::Start()
{
	Poll();
	setClients(getLoudSockets());
	readClients();
	respondClients();
	closeTimeOut();
}


// WaitForClientRequest()
// BuildRequest() //showes it in
// AssigningVirtualServerToClient()
// BuildingResponse() 
// beSendingResponse() || beReceiving() || close()

