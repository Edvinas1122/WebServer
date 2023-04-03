#include <ContentBrowser.hpp>

bool	ContentBrowser::Ready(Client &client)
{
	if (client.incomingAvailable())
	{
		if (!clientInServiceList(&client))
		{
			return (true);
		}
	}
	return (false);
};

bool	ContentBrowser::addServiceProcess(Client *client, std::string const &requested_service)
{
	if (requested_service == "POST")
		processList.push_back(ServiceProcess());
};

void	ContentBrowser::Serve(Client &client)
{
	HttpRequest	request(client.getMessage());
	std::string	rootDir = "/home/WebServer/http";

	if (request.getMethod() == "GET")
	{

		client.setInactiveTimeOutCounter(5);
		client << File("/home/WebServer/http/files/responseGET.txt").GetContentsAsString();
		client << "\r\nContent-Length: " << File((rootDir + request.getLocation()).c_str()).GetSize();
		client << "\r\n\r\n";
		client << File((rootDir + request.getLocation()).c_str());
		return ;
	}
	if (request.getMethod() == "POST")
	{
		client.setInactiveTimeOutCounter(10000);

		try {
			addClientProcess(&client, request.getBoundry());
			client << File("/home/WebServer/http/files/responseToPost.txt").GetContentsAsString();
		} catch (...) {
			client.setInactiveTimeOutCounter(0);
			client << "HTTP/1.1 500 Internal Server Error\r\n\r\n";
			std::cout << "POST request Failure" << std::endl;
		}
		return ;
	}
};

void	ContentBrowser::Handle(Client &client)
{
	if (clientInServiceList(&client) && client.incomingAvailable()) {
		findClient(&client)->Handle();
	}
};
