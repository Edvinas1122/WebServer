#include <Server.hpp>
#include <Terminal.hpp>
#include <Response.hpp>

void	printReceived(Client &client)
{
	if (client.getMessage().length()) {
		client.info();
		std::cout << client;
		client.updateTime(true);
		// client << "message received\n";
	}
}

void	handleTerminal(Client &client, Terminal *terminal)
{
	Response	response;

	if (terminal->notEmpty())
	{
		if (!client.HeaderSent()) {
			client.UpdateHeaderInfo();
			response.Build();
			client << response.get();
		}
		client << terminal->extractMessage();
	}
}

void	clearTerminalMessages(Client &client, Terminal *terminal)
{
	(void) client;
	if (terminal->notEmpty())
	{
		terminal->clearMessage();
	}
}

#include <DescendParser.hpp>
#include <configurationFileFormat.hpp>

void	startHttpServer(Server<Terminal*> &server, const char *configPath)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(configPath);
	parser.setContent(configurationFile.GetContents(removeComents));
	server.startPorts<DescendParser>(wordMatchMethod, parser);
}

int	main(void)
{
	Server<Terminal*>	httpServer;
	Terminal			terminal;

	startHttpServer(httpServer, "/home/WebServer/server.conf");
	httpServer.setAction(printReceived);
	httpServer.setAction(handleTerminal, &terminal);
	httpServer.setAction(clearTerminalMessages, &terminal);
	while (terminal.terminal_interface())
	{
		httpServer.Run();
	}
	return (0);
}