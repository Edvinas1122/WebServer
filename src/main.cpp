#include <Service.hpp>
#include <Server.hpp>
#include <Terminal.hpp>
#include <File.hpp>

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


int	main(void)
{
#ifdef TERMINAL
	Terminal			terminal;
#endif
	ContentBrowser		webSite;
	Server<Service*>	httpServer;

	startHttpServer(httpServer, "/home/WebServer/server.conf");

#ifdef TERMINAL
	httpServer.setService(&terminal);
	httpServer.setService(&webSite);

	while (terminal.terminal_interface())
#else
	while (42)
#endif
	{
		httpServer.Run();
	}
	return (0);
}