#include <ServerClasses.hpp>

void	printReceived(Client &client)
{
	if (client.getMessage().length()) {
		std::cout << client;
	}
}

void	queMessage(Client &client)
{
	client << "message received\n";
}

#include <DescendParser.hpp>
#include <configurationFileFormat.hpp>
#include <openPortSocket.hpp>

void	startHttpServer(Server<std::string> &server, const char *configPath)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(configPath);
	parser.setContent(configurationFile.GetContents(removeComents));
	server.setStartSocketMethod(openPortSocket);
	server.startPorts<DescendParser>(wordMatchMethod, parser);
}

int	main(void)
{
	Server<std::string>	httpServer;

	startHttpServer(httpServer, "/home/WebServer/server.conf");
	httpServer.setReceivingAction(printReceived);
	httpServer.setRespondingAction(queMessage);
	while (42)
	{
		httpServer.Run();
	}
}