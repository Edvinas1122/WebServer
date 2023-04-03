#include <WebSite.hpp>

void	WebSite::addServiceProcess(Client &client, HttpRequest const &request, const VirtualServer &vs)
{
	(void) vs;

	if (request.getMethod() == "POST")
		Service::SetProcess(new FileUpload(&client));
	else if (request.getMethod() == "GET")
		Service::SetProcess(new Download(&client, std::string("/home/WebServer/http") + request.getLocation()));
	else
		client << "HTTP/1.1 400 Bad Request\r\n\r\n";
};

void	WebSite::SetResponse(Client &client)
{
	HttpRequest		request(client.getMessage());

	client.info();
	addServiceProcess(client, request, virtualServers->getServer(client.getPort(), request.getHost()));
};

void	WebSite::Handle(ServiceProcess *process)
{
	if (process->Ready()) {
		process->Handle();
	}
};

void	WebSite::SetVirtualServerMap(VirtualServers *vs)
{
	this->virtualServers = vs;
}