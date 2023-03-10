#include <ServerClasses.hpp>

int	main(void)
{
	ServerManager	engine;

	engine.serverCreator("/home/WebServer/server.conf");
	engine.Info();
	while (42)
	{
		engine.Start();
	}
}