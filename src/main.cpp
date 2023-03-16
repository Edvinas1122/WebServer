#include <Server.hpp>
#include <Service.hpp>
#include <Terminal.hpp>
#include <File.hpp>
#include <HTTP.hpp>

// #define	FILE_BOUNDRY (request) (request.getHeaders().at("Content-Type").substr(request.getHeaders().at("Content-Type").find("boundary=") + 9))

void	parseRequest(Client &client, HttpRequest request)
{
	std::string get("/home/WebServer/files/http/form.html");
	// get.append(request.getLocation());
	std::cout << get << std::endl;
	if (request.getMethod() == "GET")
	{
		client.UpdateHeaderInfo();
		client.enableKeepAlive();
		client << File("/home/WebServer/files/response.txt").GetContents();
		client << "\r\nContent-Length: " << File(get.c_str()).GetSize();
		client << "\r\n\r\n";
		client << File(get.c_str());
		// client << "\r\n";
	}
	if (request.getMethod() == "POST")
	{
		client.enableKeepAlive();
		client << File("/home/WebServer/files/responseToPost.txt").GetContents();
		if (!client.HeaderSent())
		{
			client.UpdateHeaderInfo();
			client.Download("/home/WebServer/files/test.txt"); // out of map telnet exception
		}
		// client << "\r\nContent-Length: 0";
		// client << "\r\n\r\n";
	}
}

void	printReceived(Client &client)
{
	if (client.getMessage().length()) {
		if (!client.HeaderSent()) {
			client.info();
			parseRequest(client, client.getMessage());
		}
	}
	// client.Download();
	// std::cout << client;
}

// void	fileHandle(Client &client, Service *service)
// {

// }

void	handleTerminal(Client &client, Service *terminal) {

	if (terminal->Ready()) {
		if (!client.HeaderSent())
		{
			client.UpdateHeaderInfo();
			client << terminal->Serve(client.getMessage());
		}
		client.enableKeepAlive();
	}
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

	Server<Service*>	httpServer;

	startHttpServer(httpServer, "/home/WebServer/server.conf");

	httpServer.setAction(printReceived);
	// httpServer.setAction(fileHandle, &service);
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