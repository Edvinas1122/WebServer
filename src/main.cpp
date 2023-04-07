#include <Service.hpp>
#include <WebSite.hpp>
#include <Server.hpp>
#include <ProgramInterface.hpp>
#include <TelnetServer.hpp>

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

/*
	Signal Termination for freeing memory
*/
bool	signalEnd = false;

void handleSignal(int sigNum)
{
	(void) sigNum;
	std::cout << "Signal Terminate Process" << std::endl;
	signalEnd = true;
}

int	main(void)
{
#ifdef TERMINAL
	Terminal		terminal;
	TelNetServer	tns;
#endif
	VirtualServers	virtualServers;
	WebSite			webSite;
	Server			httpServer;

	// startHttpServer(httpServer, "/home/WebServer/server.conf");
	// virtualServers.parseConfigurationFile("/home/WebServer/server.conf");
	httpServer.startPort("10012");
	// httpServer.startPorts();
	// virtualServers.Info();
	// webSite.SetVirtualServerMap(&virtualServers);
	signal(SIGINT, handleSignal);
	signal(SIGQUIT, handleSignal);
#ifdef TERMINAL
	httpServer.addService(&tns);
#endif
	httpServer.addService(&webSite);

#ifdef TERMINAL
	while (!terminal.EndProgram() && !signalEnd)
#else
	while (!signalEnd)
#endif
	{
		httpServer.Run();
#ifdef TERMINAL
		terminal.Input();
#endif
	}
	return (0);
}