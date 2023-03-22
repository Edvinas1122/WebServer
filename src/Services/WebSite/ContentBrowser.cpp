#include <ContentBrowser.hpp>

bool	ContentBrowser::Ready(Client &client)
{
	if (client.incomingAvailable())
	{
		if (client.checkFlag(FILE_TRANSFERED))
		{
			return (true);
		}
	}
	return (false);
}

void	ContentBrowser::Serve(Client &client)
{
	std::string get("/home/WebServer/http");
	// std::cout << get << std::endl;
	HttpRequest	request(client.getMessage());

	get.append(request.getLocation());
	if (request.getMethod() == "GET")
	{
		client.setInactiveTimeOutCounter(5);
		client << File("/home/WebServer/http/files/responseGET.txt").GetContentsAsString();
		client << "\r\nContent-Length: " << File(get.c_str()).GetSize();
		client << "\r\n\r\n";
		client << File(get.c_str());
		// client << "\r\n";
	}
	if (request.getMethod() == "POST")
	{
		client.setInactiveTimeOutCounter(1);
		client << File("/home/WebServer/http/files/responseToPost.txt").GetContentsAsString();
		client.Download("/home/WebServer/http/test.txt"); // out of map telnet exception
		std::cout << "Receiving file" << std::endl;
	}
};

void	ContentBrowser::Handle(Client &client)
{
	if (client.incomingAvailable() && !client.checkFlag(FILE_DOWNLOADING)) {
		std::cout << "Messaging Client socket id" << client.getSocket() << std::endl;
		std::cout << client;
	}
}

// void	ContentBrowser::gate(Client &client)
// {
// 	traceClient(client);
// }

// bool	ServiceProcessTrace::isClient(const Client &client)
// {
// 	return ((id == client.getFlags()));
// }


// void	ServiceTracer::traceClient(Client &client)
// {
// 	st_iterator it = findClient(client);

// 	if (it == processList.end())
// 		client.setFlags(processList.size());
// 	else
// 		client.setFlags(10);
// }

// ServiceTracer::st_iterator	ServiceTracer::findClient(const Client &client)
// {
// 	st_iterator	iterator = processList.begin();

// 	while (iterator != processList.end())
// 	{
// 		(*iterator).isClient(client);
// 		iterator++;
// 	}
// 	return (iterator);
// }
