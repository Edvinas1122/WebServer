#include <Service.hpp>
// #include <WebSite.hpp>
#include <Server.hpp>
#include <Terminal.hpp>

// #include <DescendParser.hpp>
// #include <configurationFileFormat.hpp>

// void	startHttpServer(Server &server, const char *configPath)
// {
// 	DescendParser		parser;
// 	FileProcessor		configurationFile;

// 	configurationFile.Open(configPath);
// 	parser.setContent(configurationFile.GetContents(removeComents));
// 	server.startPorts<DescendParser>(wordMatchMethod, parser);
// }

// #include <VirtualServer.hpp>

int	main(void)
{
#ifdef TERMINAL
	Terminal	terminal;
#endif
	// VirtualServers	virtualServers;
	// WebSite			webSite;
	Server			httpServer;

	// startHttpServer(httpServer, "/home/WebServer/server.conf");
	// virtualServers.parseConfigurationFile("/home/WebServer/server.conf");
	httpServer.startPort("10012");
	// virtualServers.Info();
	// webSite.SetVirtualServerMap(&virtualServers);
#ifdef TERMINAL
	httpServer.addService(&terminal);
#endif
	// httpServer.addService(&webSite);

#ifdef TERMINAL
	while (42)
#else
	while (42)
#endif
	{
		httpServer.Run();
	}
	return (0);
}