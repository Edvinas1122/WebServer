#include <ContentBrowser.hpp>

bool	ContentBrowser::Ready(Client &client)
{
	if (client.incomingAvailable())
	{
		if (client.checkFlag(FILE_TRANSFERED) && !clientInServiceList(&client))
		{
			return (true);
		}
	}
	return (false);
};

void	ContentBrowser::Serve(Client &client)
{
	// std::cout << get << std::endl;
	HttpRequest	request(client.getMessage());

	if (request.getMethod() == "GET")
	{
		std::string	rootDir = "/home/WebServer/http";

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
		client << File("/home/WebServer/http/files/responseToPost.txt").GetContentsAsString();
		// client.Download("/home/WebServer/http/test.txt"); // out of map telnet exception
		try {
			addDownload(&client, "/home/WebServer/http/test.txt", request.getBoundry());
		} catch (...) {
			addDownload(&client, "/home/WebServer/http/test.txt");
		}
		std::cout << "Receiving file" << std::endl;
		return ;
	}
};

// void	ContentBrowser::Handle(Client &client)
// {
// 	if (client.incomingAvailable() && !client.checkFlag(FILE_DOWNLOADING)) {
// 		std::cout << "Messaging Client socket id" << client.getSocket() << std::endl;
// 		std::cout << client;
// 	} else if (client.incomingAvailable() && client.checkFlag(FILE_DOWNLOADING))
// 	{
// 		pullToFile(client);
// 	}
// };

void	ContentBrowser::Handle(Client &client)
{
	if (clientInServiceList(&client) && client.incomingAvailable())
	{
		std::cout << "writing to file input" << std::endl;
		writeBufferToFile(&client);
		if (fileEnd(&client)) {
			closeDownload(&client);
			client.setInactiveTimeOutCounter(0);
			std::cout << "file received" << std::endl;
		}
		// client << "\r\n\r\n";
	}
	if (client.incomingAvailable())
		std::cout << client;
};