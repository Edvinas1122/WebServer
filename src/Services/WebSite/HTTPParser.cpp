#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

bool	HTTPParser::Handle()
{
	if (!theConnection().downloadBufferReady() &&
		!theConnection().uploadBufferReady() && theConnection().getElapsedTime() > heartBeatRate)
		return (HeartBeat());
	else
	{
		try {
			return (BufferRequest::Handle());
		} catch (...)
		{
			SetFollowingProcess(ErrorRespone(500));
			return (false);
		}
	}
}

bool	HTTPParser::HeartBeat()
{
	theConnection() << "200 OK\r\nConnection: keep-alive\r\n\r\n";
	return (true);
}

#define MAX_REQUEST_LEN 2048

bool	HTTPParser::RequestCompleted(std::string const &request)
{
	if (request.length() > MAX_REQUEST_LEN)
		throw (std::exception());
	return (HttpRequest(request).Completed());
}

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

#define MAX_UPLOAD_VIA_PUT_SIZE 13000000000 // 13 GB

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
		setMaxReceiveSize(virtualServer->maxRecevieSize(HttpRequest(request).getLocation().getDir(), HttpRequest(request).getLocation().getFileName()));
		if (request.find("_method=DELETE") != std::string::npos)
			return (handleDeleteRequest(dir, request));
		return (handleUploadRequest(dir, request));
	}
	else if (HttpRequest(request).getMethod() == "PUT")
	{
		setMaxReceiveSize(MAX_UPLOAD_VIA_PUT_SIZE);
		return (handleUploadRequest(dir, request));
	}
	return (new TerminateProcess(&theConnection()));
};

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
	if (virtualServer->isCGI(UrlQuery(dir).getFileExtension()))
	{
		theConnection() << headerMessage(0, 200);
		return (handleCGIExecution(dir, request));
	}
	if (request.getProtocolVersion() == "HTTP/1.0" || request.getKeepAlive() == "close") {
		theConnection() << headerMessage(0, 200);
		return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), dir));
	}
	theConnection() << headerMessage(0, 200);
	return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), dir));
	// theConnection() << headerMessage(1, 200, File(dir.c_str()).GetSize());
	// return (new HTTPFileSend(*this, new HTTPParser(*this), dir));
}

ExecuteFile	*HTTPParser::handleCGIExecution(std::string const &dir, HttpRequest const &request)
{
	std::string	cgiExecutableDir = virtualServer->CGIexecutableDir(UrlQuery(dir).getFileExtension());

	// ExecuteFile	*exec = new ExecuteFile(&theConnection(), new HTTPParser(*this), cgiExecutableDir, dir);
	ExecuteFile	*exec = new ExecuteFile(&theConnection(), new TerminateProcess(&theConnection()), cgiExecutableDir, dir);
	exec->SetEnvVariable(setVar("REQUEST_METHOD", request.getMethod()));
	exec->SetEnvVariable(setVar("SERVER_PROTOCOL", request.getProtocolVersion().substr(0, 8)));
	exec->SetEnvVariable(setVar("PATH_INFO", request.getLocation().getCGIPathInfo()));
	exec->SetEnvVariable(setVar("SCRIPT_NAME", request.getLocation().getFileName()));
	return (exec);
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

static Buffer	removeHeader(std::string const &request)
{
	Buffer	requestBuffered;

	requestBuffered	<< request.substr(request.find("\r\n\r\n") + 4);
	return (requestBuffered);
}

static bool	chunkedFileUploadRequest(HttpHeaders const requestHeaders)
{
	if (requestHeaders.find("Transfer-Encoding") != requestHeaders.end())
	{
		if (requestHeaders.at("Transfer-Encoding") == "chunked")
			return (true);
	}
	return (false);
}

#include <BufferedReceiveTypes.hpp>

#define	DEFAULT_TMP_PATH "/home/http/tmp/cgi_tmp"

ServiceProcess		*HTTPParser::handleUploadRequest(std::string const &dir, HttpRequest const &request)
{
	HTTPBufferReceive	*process;
	// size_t	fileSize = atoi((request.getHeaders().at("Content-Length").c_str()));

	if (virtualServer->isCGI(UrlQuery(dir).getFileExtension()))
	{
		ExecuteFile		*cgi = handleCGIExecution(dir, request);
		std::string		tmpFile = updateDirIfFileExists(DEFAULT_TMP_PATH);

		cgi->FileIntoExec(tmpFile);
		process = new HTTPLenChunkedFileReceive(*this, cgi, tmpFile);

	}
	else if (chunkedFileUploadRequest(request.getHeaders()))
		process = new HTTPLenChunkedFileReceive(*this, new HTTPFileReceiveReport(*this, new TerminateProcess(&theConnection())), updateDirIfFileExists(dir));
	else
		process = new HTTPDelimiterChunkedFileReceive(*this, new HTTPFileReceiveReport(*this, new HTTPParser(*this)), request.getBoundry(), dir);
	*process << removeHeader(std::string(request));
	return (process);
}

// ServiceProcess	*VirtualServerHTTPParser::RequestParse(std::string const &request)
// {
// 	ServiceProcess	*process;
// 	virtualServer = virtualServers.getServerConfig(theConnection().getPort(), request.getHost());

// 	process = HTTPParser::RequestParse(request);

// };

void	HTTPParser::setMaxReceiveSize(const size_t value)
{
	received = 0;
	max_receive_size = value;
}

bool	HTTPParser::allowInsert(const size_t newInsertSize)
{
	received += newInsertSize;
	return ((received < max_receive_size));
}