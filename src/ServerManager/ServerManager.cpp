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


void	ServerManager::Start()
{
	Observer::Poll();
	ClientsQue::setClients(getLoudSockets());
	ClientsQue::readClients();  // process inbound
	ClientsQue::respondClients(); // process outgoing
	ClientsQue::closeTimeOut();
}


// WaitForClientRequest()
// BuildRequest() //showes it in
// AssigningVirtualServerToClient()
// BuildingResponse() 
// beSendingResponse() || beReceiving() || close()

