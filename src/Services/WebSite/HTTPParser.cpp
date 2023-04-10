#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

# define DIR_LISTING_ALLOWED true
# define TEST_LOCATION_DEFAULT_RESPONSES(x) ( x == "/home/WebServer/http/")

const std::string	HTTPParser::headerMessage(const int &method_version, const int &code, const int content_len, bool keep_alive)
{
	std::stringstream	message;

	message << "HTTP/1." << method_version << " " << code << " " << getHttpExplanation(code) << "\r\n";
	message << "Connection: " << (keep_alive ? "keep-alive" : "close") << "\r\n";
	if (keep_alive)
		message << "Content-Length: " << content_len << "\r\n";
	// message << "Date: ";
	message << "\r\n";
	return (message.str());
}

ServiceProcess	*HTTPParser::ErrorRespone(const int code, bool close_connection)
{
	unsigned int	version = (close_connection ? 1 : 0);

	if ((virtualServer->errorPage(code)).empty())
	{
		if (close_connection)
		{
			theConnection() << headerMessage(version, code, getHttpExplanation(code).length(), !close_connection) << getHttpExplanation(code);
			return (new TerminateProcess(&theConnection()));
		} else {
			theConnection() << headerMessage(version, code, getHttpExplanation(code).length(), close_connection) << getHttpExplanation(code);
			return (new HTTPParser(*this));
		}
	} else {
		size_t	fileSize = File(virtualServer->errorPage(code).c_str()).GetSize();
		if (close_connection)
		{
			theConnection() << headerMessage(version, code, fileSize, !close_connection) << getHttpExplanation(code);
			return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), virtualServer->errorPage(code)));
		} else {
			theConnection() << headerMessage(version, code, fileSize, close_connection) << getHttpExplanation(code);
			return (new HTTPFileSend(*this, new HTTPParser(*this), virtualServer->errorPage(code)));
		}
	}
}

ServiceProcess	*HTTPParser::RequestParse(std::string const &request)
{
	std::string	dir = virtualServer->getSystemPath(HttpRequest(request).getLocation().getDir(), HttpRequest(request).getLocation().getFileName());

	if (HttpRequest(request).getMethod() == "GET")
	{
		if (!virtualServer->methodPermited(HttpRequest(request).getLocation().getDir(), "GET"))
			return (ErrorRespone(405));
		if (dir.empty())
		{
			theConnection() << virtualServer->getRedirectMessage(HttpRequest(request).getLocation().getDir());
			return (new HTTPParser(*this));
		}
		if (!Access(dir)) {
			return (ErrorRespone(404));
		}
		if (!isFile(dir) && DIR_LISTING_ALLOWED) {
			theConnection() << HTTPHeaderDirOK(dir, std::string("http://") + HttpRequest(request).getHost() + HttpRequest(request).getLocation());
			return (new HTTPParser(*this));
		}
		if (virtualServer->isCGI(UrlQuery(dir).getFileExtension()))
		{
			std::string	cgiExecutableDir = virtualServer->CGIexecutableDir(UrlQuery(dir).getFileExtension());

			theConnection() << "HTTP/1.1 200 OK\r\nConnection: close\r\n";
			return (new ExecuteFile(&theConnection(), new TerminateProcess(&theConnection()), cgiExecutableDir, dir));
		}
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
			return (handleDeleteRequest(dir, request));
	}
	return (new TerminateProcess(&theConnection()));
};

ServiceProcess	*HTTPParser::handleDeleteRequest(std::string const &dir, HttpRequest const &request)
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