#include "WebSite.hpp"

ServiceProcess	*WebSite::RequestParse(Connection *connection, std::string const &request)
{
	if (HttpRequest(request).Validate())
		return (new HTTPParser(connection, NULL));
	return (NULL);
}

bool	WebSite::DetermineIfIdleProcessType(ServiceProcess *process)
{
	if (dynamic_cast<HTTPParser*>(process) != NULL)
		return (true);
	return (false);
};