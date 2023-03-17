#include <Service.hpp>

bool	ContentBrowser::Ready(Client &client)
{
	return (client.incomingAvailable() && !client.uploadBufferNotEmpty());
}

void	ContentBrowser::Serve(Client &client)
{
	std::string get("/home/WebServer/files/http/form.html");
	// std::cout << get << std::endl;
	HttpRequest	request(client.getMessage());


	if (request.getMethod() == "GET")
	{
		client.setInactiveTimeOutCounter(5);
		client << File("/home/WebServer/http/files/response.txt").GetContentsAsString();
		client << "\r\nContent-Length: " << File(get.c_str()).GetSize();
		client << "\r\n\r\n";
		client << File(get.c_str());
		// client << "\r\n";
	}
	if (request.getMethod() == "POST")
	{
		client.setInactiveTimeOutCounter(5);
		client << File("/home/WebServer/files/responseToPost.txt").GetContentsAsString();
		client.Download("/home/WebServer/files/test.txt"); // out of map telnet exception
		// client << "\r\nContent-Length: 0";
		// client << "\r\n\r\n";
	}
};

void	ContentBrowser::Handle(Client &client)
{
	std::cout << client;
}