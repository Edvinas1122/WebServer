#include "WebSite.hpp"
#include <TCP.hpp>

ServiceProcess	*WebSite::RequestParse(Connection *connection, std::string const &request)
{
	TCP	*_connection = dynamic_cast<TCP*>(connection);

	if (_connection == NULL)
		return (NULL);
	if (HttpRequest(request).Validate())
		return (new HTTPParser(connection, virtualServers->getServer("", _connection->getPort())));
	return (NULL);
}

bool	WebSite::DetermineIfIdleProcessType(ServiceProcess *process)
{
	if (dynamic_cast<HTTPParser*>(process) != NULL)
		return (true);
	return (false);
};