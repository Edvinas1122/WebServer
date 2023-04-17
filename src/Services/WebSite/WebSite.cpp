#include "WebSite.hpp"
#include <TCP.hpp>

ServiceProcess	*WebSite::RequestParse(Connection *connection, std::string const &request)
{
	TCP				*_connection = dynamic_cast<TCP*>(connection);
	VirtualServer 	*virtual_server;

	if (_connection == NULL)
		return (NULL);
	if (HttpRequest(request).Validate())
	{
		virtual_server = virtualServers->getServer(_connection->getPort(), HttpRequest(request).getHost());
		if (virtual_server == NULL)
		{
			throw std::exception();
			// return (new HTTPParser(connection,  virtualServers->getDefault()));
		}
		return (new HTTPParser(connection, virtual_server));
	}
	return (NULL);
}

bool	WebSite::DetermineIfIdleProcessType(ServiceProcess *process)
{
	if (dynamic_cast<HTTPParser*>(process) != NULL)
		return (true);
	return (false);
};