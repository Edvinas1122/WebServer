#include <Service.hpp>
#include <WebSite.hpp>
#include <Server.hpp>
#include <ProgramInterface.hpp>
#include <TelnetServer.hpp>
#include <IRC.hpp>

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

int	main(int argc, char **args)
{
#ifdef TERMINAL
	bool			giveInfoToIRC = false;
	Terminal		terminal;
	TelNetServer	TelNetController;
	IRCService		irc;
#endif
	VirtualServers	virtualServers;
	WebSite			webSite;
	Server			httpServer;

	signal(SIGINT, handleSignal); // setSig for ending leak free
	signal(SIGQUIT, handleSignal);
	try {
		if (argc == 1)
			virtualServers.parseConfigurationFile("/home/WebServer/server.conf");
		else
			virtualServers.parseConfigurationFile(args[1]);
		// virtualServers.Info(); // 
	} catch (...) {
		std::cout << "Configuration Read Failure" << std::endl;
		return (EXIT_FAILURE);
	}
	try {
		httpServer.startPorts(virtualServers.getPortList());
	} catch (...) {
		std::cout << "Port Bind Failure" << std::endl;
		return (EXIT_FAILURE);
	}
	httpServer.infoPorts();
	webSite.SetVirtualServerMap(&virtualServers);
#ifdef TERMINAL
	httpServer.addService(&TelNetController);
	httpServer.addService(&irc);
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
		httpServer.CommandParse(terminal.DataGet("command"));
		if (giveInfoToIRC && webSite.newIncame())
		{
			terminal.addSystemMessage("IRC", "messages", webSite.connectionsInfo());
			terminal.addSystemMessage("IRC", "messages", httpServer.infoConnections());
		}
		if (terminal.DataGet("ConnectionInfo") == "get")
		{
			giveInfoToIRC ? giveInfoToIRC = false : giveInfoToIRC = true;
		}
#endif
	}
	std::cout << "Exiting..." << std::endl;
	return (EXIT_SUCCESS);
}
