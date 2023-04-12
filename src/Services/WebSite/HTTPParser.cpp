#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

# define DIR_LISTING_ALLOWED true
# define TEST_LOCATION_DEFAULT_RESPONSES(x) ( x == "/home/WebServer/http/")

const std::string	HTTPParser::headerMessage(const int &method_version, const int &code, const size_t content_len)
{
	std::stringstream	message;

	message << "HTTP/1." << method_version << " " << code << " " << getHttpExplanation(code) << "\r\n";
	message << "Connection: " << ((content_len != std::numeric_limits<size_t>::max()) ? "keep-alive" : "close") << "\r\n";
	if (content_len != std::numeric_limits<size_t>::max())
		message << "Content-Length: " << content_len << "\r\n";
	// message << "Date: ";
	message << "\r\n";
	return (message.str());
}

ServiceProcess	*HTTPParser::ErrorRespone(const int code, bool close_connection)
{
	unsigned int	version = (close_connection ? 0 : 1);

	if ((virtualServer->errorPage(code)).empty())
	{
		if (close_connection)
		{
			theConnection() << headerMessage(version, code) << getHttpExplanation(code);
			return (new TerminateProcess(&theConnection()));
		} else {
			theConnection() << headerMessage(version, code, getHttpExplanation(code).length()) << getHttpExplanation(code);
			return (new HTTPParser(*this));
		}
	} else {
		size_t	fileSize = File(virtualServer->errorPage(code).c_str()).GetSize();
		if (close_connection)
		{
			theConnection() << headerMessage(version, code) << getHttpExplanation(code);
			return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), virtualServer->errorPage(code)));
		} else {
			theConnection() << headerMessage(version, code, fileSize) << getHttpExplanation(code);
			return (new HTTPFileSend(*this, new HTTPParser(*this), virtualServer->errorPage(code)));
		}
	}
}

ServiceProcess	*HTTPParser::RequestParse(std::string const &request)
{
	std::string	dir = virtualServer->getSystemPath(HttpRequest(request).getLocation().getDir(), HttpRequest(request).getLocation().getFileName());

	if (dir.empty()) { // handles Redirect
		theConnection() << virtualServer->getRedirectMessage(HttpRequest(request).getLocation().getDir());
		return (new HTTPParser(*this));
	}
	if (!virtualServer->methodPermited(HttpRequest(request).getLocation().getDir(), HttpRequest(request).getMethod()))
		return (ErrorRespone(405));
	if (HttpRequest(request).getMethod() == "GET")
		return (handleGetRequest(dir, request));
	else if (HttpRequest(request).getMethod() == "POST")
	{
		max_receive_size = virtualServer->maxRecevieSize();
		if (!isFile(dir))
			return (handleUploadRequest(dir, request));
		if (request.find("_method=DELETE") != std::string::npos)
			return (handleDeleteRequest(dir, request));
		// if () CGI
	}
	else if (HttpRequest(request).getMethod() == "PUT")
	{
		max_receive_size = virtualServer->maxRecevieSize();
		if (!isFile(dir))
			return (handleUploadRequest(dir, request));
	}
	return (new TerminateProcess(&theConnection()));
};

const std::string	setVar(std::string const &key, std::string const &value)
{
	if (value.empty())
		return (key);
	return (key + "=" + value);
}

ServiceProcess	*HTTPParser::handleGetRequest(std::string const &dir, HttpRequest const &request)
{
	if (!Access(dir))
		return (ErrorRespone(404));
	if (!isFile(dir)) { // dirrectory listing handle
		if (virtualServer->dirListingPermited(request.getLocation().getDir()))
		{
			std::string	url_link_to_this_server = std::string("http://") + request.getHost() + request.getLocation();
			std::string	DirList = dirInfoHTTPFormat(dir.c_str(), url_link_to_this_server.c_str(), virtualServer->methodPermited(request.getLocation().getDir(), "POST"));

			theConnection() << headerMessage(1, 200, DirList.length());
			theConnection() << DirList;
			return (new HTTPParser(*this));
		} else
			return (ErrorRespone(200));
	}
	if (virtualServer->isCGI(UrlQuery(dir).getFileExtension())) { // Handle CGI
		std::string	cgiExecutableDir = virtualServer->CGIexecutableDir(UrlQuery(dir).getFileExtension());

		ExecuteFile	*exec = new ExecuteFile(&theConnection(), new TerminateProcess(&theConnection()), cgiExecutableDir, dir);
		exec->SetEnvVariable(setVar("REQUEST_METHOD", request.getMethod()));
		exec->SetEnvVariable(setVar("SERVER_PROTOCOL", request.getProtocolVersion().substr(0, 8)));
		exec->SetEnvVariable(setVar("PATH_INFO", request.getLocation().getCGIPathInfo()));
		// exec->SetEnvVariable(setVar("PATH_INFO", request.getLocation().getPath()));
		exec->SetEnvVariable(setVar("SCRIPT_NAME", request.getLocation().getFileName()));
		theConnection() << headerMessage(0, 200);
		return (exec);
	}
	if (request.getProtocolVersion() == "HTTP/1.0" || request.getKeepAlive() == "close") {
		theConnection() << headerMessage(0, 200);
		std::cout << "requested with close" << std::endl;
		return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), dir));
	}
	theConnection() << headerMessage(1, 200, File(dir.c_str()).GetSize());
	return (new HTTPFileSend(*this, new HTTPParser(*this), dir));
}

ServiceProcess	*HTTPParser::handleDeleteRequest(std::string const &dir, HttpRequest const &request)
{
	if (access(dir.c_str(), W_OK) == 0)
	{
		std::remove(dir.c_str());
		theConnection() << headerMessage(1, 200, 18 + HttpRequest(request).getLocation().getFileName().length());
		theConnection() << "Resource Deleted: " << HttpRequest(request).getLocation().getFileName();
		return (new HTTPParser(*this));
	}
	return (ErrorRespone(403));
}

Buffer	*removeHeader(Buffer *tmp)
{
	std::string	header_tmp;

	*tmp >> header_tmp;
	*tmp = tmp->substr(header_tmp.find("\r\n\r\n") + 4);
	return (tmp);
}

void	appendToBuffer(Connection *connection, Buffer *buffer)
{
	Buffer	tmp;

	*connection >> tmp;
	*buffer << tmp;
}

ServiceProcess		*HTTPParser::handleUploadRequest(std::string const &dir, HttpRequest const &request)
{
	try {
		// size_t	fileSize = atoi((request.getHeaders().at("Content-Length").c_str()));
		HTTPBufferReceive	*process;
		Buffer				requestBuffered;

		process = new HTTPDelimiterChunkedFileReceive(*this, new HTTPFileReceiveReport(*this), request.getBoundry(), dir);
		requestBuffered << std::string(request);
		appendToBuffer(&theConnection(), &requestBuffered);
		*process << *removeHeader(&requestBuffered);
		return (process);
	} catch (...) {
		return (ErrorRespone(500));
	}
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
		return (BufferRequest::Handle());
	}
}

bool	HTTPParser::HeartBeat()
{
	theConnection() << "200 OK\r\nConnection: keep-alive\r\n";
	return (true);
}

#define MAX_REQUEST_LEN 2048

bool	HTTPParser::RequestCompleted(std::string const &request)
{
	if (request.length() > MAX_REQUEST_LEN)
		throw (std::exception());
	return (HttpRequest(request).Completed());
}