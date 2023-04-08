#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

# define IS_CGI false
# define CGI NULL
# define DIR_LISTING_ALLOWED true
# define TEST_LOCATION_DEFAULT_RESPONSES(x) ( x == "/home/WebServer/http/")

ServiceProcess	*HTTPParser::RequestParse(std::string const &request)
{
	std::string	dir = virtualServer->getSystemRoot(HttpRequest(request).getLocation().getDir()) + HttpRequest(request).getLocation().getFileName();

	// TEST_FOR_PROXY_DIRS_(dir)
		// dir modify 
	if (HttpRequest(request).getMethod() == "GET")
	{
		if (!Access(dir)) {
			theConnection () << "HTTP/1.1 404 Not Found\r\nContent-Length: 14\r\n\r\nPage Not Found\n";
			return (new HTTPParser(*this));
		}
		if (!isFile(dir) && TEST_LOCATION_DEFAULT_RESPONSES(dir)) { // test for dir default responses
			std::string	defaultResponse = "index.html";
			dir += defaultResponse;
		}
		if (!isFile(dir) && DIR_LISTING_ALLOWED) {
			theConnection() << HTTPHeaderDirOK(dir, std::string("http://46.101.198.64:10012") + HttpRequest(request).getLocation());
			return (new HTTPParser(*this));
		}
		if (IS_CGI)
			return (CGI);
		theConnection() << HTTPHeaderFileOK(dir);
		if (HttpRequest(request).getProtocolVersion() == "HTTP/1.0" || HttpRequest(request).getKeepAlive() == "close") {
			std::cout << "requested with close" << std::endl;
			return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), dir));
		}
		return (new HTTPFileSend(*this, new HTTPParser(*this), dir));
	} else if (HttpRequest(request).getMethod() == "POST")
	{
		if (!Access(dir))
		{
			theConnection() << "HTTP/1.1 404 Not Found\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
			return (new HTTPParser(*this));
		}
		if (!isFile(dir) && allowedDir(dir))
		{
			try {
				size_t	fileSize = atoi((HttpRequest(request).getHeaders().at("Content-Length").c_str()));
				HTTPFileReceive	*process;
				Buffer			requestBuffered;

				std::cout << theConnection() << std::endl;
				std::cout << "File receive size: " << fileSize << std::endl;
				// process = new HTTPFileReceive(*this, new HTTPFileReceiveReport(*this, new HTTPParser(*this)),
				process = new HTTPFileReceive(*this, new HTTPFileReceiveReport(*this, new TerminateProcess(&theConnection())),
								dir + HttpRequest(request).getFilename(), fileSize, HttpRequest(request).getBoundry());
				theConnection() >> requestBuffered;
				*process << requestBuffered;
				return (process);
			} catch (...) {
				theConnection() << "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 14\r\n\r\nInternal Error";
			}
		}
		if (request.find("_method=DELETE") != std::string::npos)
			return (deleteRequest(dir, request));
	}
	return (new TerminateProcess(&theConnection()));
};

ServiceProcess	*HTTPParser::deleteRequest(std::string const &dir, HttpRequest const &request)
{
	if (access(dir.c_str(), W_OK) == 0)
	{
		std::remove(dir.c_str());
		theConnection() << "HTTP/1.1 200 OK\r\nContent-Length: " << 18 + HttpRequest(request).getLocation().getFileName().length() << "\r\n\r\nResource Deleted: " << HttpRequest(request).getLocation().getFileName();
	}
	else
		theConnection() << "HTTP/1.1 403 Forbidden\r\nContent-Length: 16\r\n\r\nRequest Forbiden";
	return (new HTTPParser(*this));
}

// ServiceProcess	*VirtualServerHTTPParser::RequestParse(std::string const &request)
// {
// 	ServiceProcess	*process;
// 	virtualServer = virtualServers.getServerConfig(theConnection().getPort(), request.getHost());

// 	process = HTTPParser::RequestParse(request);

// };

/*
	test idle connections 
	risk of wrong response in theory
	recomended to use connection id like IP or MAC to determine
	same connection source 
*/

bool	HTTPParser::Handle()
{
	if (!theConnection().downloadBufferReady() &&
		!theConnection().uploadBufferReady() && theConnection().getElapsedTime() > heartBeatRate)
		return (HeartBeat());
	else
	{
		return (MasterProcess::Handle());
	}
}

bool	HTTPParser::HeartBeat()
{
	std::cout << "HeartBeat Connection..." << std::endl;
	theConnection() << "200 OK\r\nConnection: keep-alive\r\n";
	return (true);
}