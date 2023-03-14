#include <Server.hpp>
#include <Terminal.hpp>
#include <Response.hpp>
#include <Parser.hpp>
#include <File.hpp>

void	parseRequest(Client &client, std::string request)
{
	Parser	parser;
	File	file;

	parser.setObject(request);
	if (parser.getWord(0, 0) == "GET")
	{
		if (parser.getWord(1, 0) == "/")
			file.Open("/home/WebServer/files/example.html");
		else
			file.Open("/home/WebServer/files/test.txt");
		client << file;
	}
}

void	printReceived(Client &client)
{
	if (client.getMessage().length()) {
		client.info();
		parseRequest(client, client.getMessage());
		std::cout << client;
		// client << "message received\n";
	}
}

void	handleTerminal(Client &client, Terminal *terminal)
{
	// Response	response;

	if (terminal->notEmpty())
	{
		if (!client.HeaderSent()) {
			client.UpdateHeaderInfo();
			// response.Build();
			// client << response.get();
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
#ifdef TERMINAL
	Terminal			terminal;
#endif

	startHttpServer(httpServer, "/home/WebServer/server.conf");

	httpServer.setAction(printReceived);
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