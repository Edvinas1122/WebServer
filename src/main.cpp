#include <ServerClasses.hpp>

int	main(void)
{
	ServerManager	engine;

	engine.serverCreator("/home/server_/server.conf");
	engine.PrintServers();
	while (42)
	{
		engine.checkPortSockets();
	}
}