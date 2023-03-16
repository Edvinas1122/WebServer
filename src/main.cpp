#include <Service.hpp>
#include <Server.hpp>
#include <Terminal.hpp>
#include <File.hpp>
#include <HTTP.hpp>

// void	printReceived(Client &client, Service *webSite)
// {
// 	(void) webSite;
// 	if (client.getMessage().length()) {
// 		if (!client.HeaderSent()) {
// 			client.info();
// 			parseRequest(client, client.getMessage());
// 		}
// 	}
// 	// client.Download();
// 	// std::cout << client;
// }

void	handleTerminal(Client &client, Service *terminal) {

	if (terminal->Ready(client)) {
		terminal->Serve(client);
		client.setInactiveTimeOutCounter(10);
	}
	std::cout << client;
}

void	clearTerminalMessages(Client &client, Service *terminal)
{
	(void) client;
	terminal->Handle();
}

#include <DescendParser.hpp>
#include <configurationFileFormat.hpp>

void	startHttpServer(Server<Service*> &server, const char *configPath)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(configPath);
	parser.setContent(configurationFile.GetContents(removeComents));
	server.startPorts<DescendParser>(wordMatchMethod, parser);
}

#include <Service.hpp>

int	main(void)
{
#ifdef TERMINAL
	Terminal			terminal;
#endif
	// ContentBrowser		webSite;
	Server<Service*>	httpServer;

	startHttpServer(httpServer, "/home/WebServer/server.conf");

	// httpServer.setAction(printReceived, &webSite);
#ifdef TERMINAL
	httpServer.setAction(handleTerminal, &terminal);
	httpServer.setAction(clearTerminalMessages, &terminal);
	while (terminal.terminal_interface())
#else
	while (42)
#endif
	{
		httpServer.Run();
	}
	return (0);
}