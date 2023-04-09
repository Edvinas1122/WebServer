#include <Service.hpp>
#include <WebSite.hpp>
#include <Server.hpp>
#include <ProgramInterface.hpp>
#include <TelnetServer.hpp>

/*
	Signal Termination for exit with freeing allocated memory
*/
static bool	signalEnd = false;

static void handleSignal(int sigNum)
{
	(void) sigNum;
	std::cout << "\nSignal Terminate Process" << std::endl;
	signalEnd = true;
}

int	main(void)
{
#ifdef TERMINAL
	Terminal		terminal;
	TelNetServer	TelNetController;
#endif
	VirtualServers	virtualServers;
	WebSite			webSite;
	Server			httpServer;

	signal(SIGINT, handleSignal); // setSig for ending leak free
	signal(SIGQUIT, handleSignal);
	try {
		virtualServers.parseConfigurationFile("/home/WebServer/server.conf");
		// virtualServers.Info(); // 
	} catch (...) {
		std::cout << "Configuration Read Failure" << std::endl;
		return (EXIT_FAILURE);
	}
	httpServer.startPorts(virtualServers.getPortList());
	httpServer.infoPorts();
	webSite.SetVirtualServerMap(&virtualServers);
#ifdef TERMINAL
	httpServer.addService(&TelNetController);
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
	std::cout << "Exiting..." << std::endl;
	return (EXIT_SUCCESS);
}