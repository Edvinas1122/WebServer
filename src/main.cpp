#include <Server.hpp>
#include <Terminal.hpp>

void	printReceived(Client &client)
{
	if (client.getMessage().length()) {
		std::cout << client;
		client << "message received\n";
	}
}

void	handleTerminal(Client &client, Terminal terminal)
{
	terminal.terminal_interface();
	terminal.extractMessage();
	if (terminal.notEmpty())
	{
		client << terminal.extractMessage();
		terminal.clearMessage();
	}
}

// void	queMessage(Client &client)
// {
// 	client << "message received\n";
// }

#include <DescendParser.hpp>
#include <configurationFileFormat.hpp>

void	startHttpServer(Server<Terminal> &server, const char *configPath)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(configPath);
	parser.setContent(configurationFile.GetContents(removeComents));
	// server.setStartSocketMethod(openPortSocket);
	server.startPorts<DescendParser>(wordMatchMethod, parser);
}



int	main(void)
{
	Server<Terminal>	httpServer;
	Terminal			terminal;

	startHttpServer(httpServer, "/home/WebServer/server.conf");
	httpServer.setAction(printReceived);
	httpServer.setAction(handleTerminal, terminal);
	while (42)
	{
		httpServer.Run();
	}
}