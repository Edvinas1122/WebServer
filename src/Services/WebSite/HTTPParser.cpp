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

std::string	headerUpgrade(HttpRequest const &request)
{
	std::string	message;

	message += "HTTP/1.1 101 Switching Protocols\r\n" 
				"Upgrade: websocket\r\n"
				"Connection: Upgrade\r\n"
				"Sec-WebSocket-Accept: " + generateWebSocketAccept(request.UpgradeKey()) + "\r\n"
				// "Sec-WebSocket-Protocol: chat\r\n"
				// "Sec-WebSocket-Extensions: permessage-deflate\r\n"
				"\r\n";
	return (message);
}

ServiceProcess	*HTTPParser::RequestParse(std::string const &request)
{
	std::string	dir = virtualServer->getSystemPath(HttpRequest(request).getLocation().getDir(), HttpRequest(request).getLocation().getFileName());

	if (dir.empty()) { // handles Redirect
		theConnection() << virtualServer->getRedirectMessage(HttpRequest(request).getLocation().getDir(), HttpRequest(request).getLocation());
		return (new HTTPParser(*this));
	}
	if (HttpRequest(request).UpgradeRequest())
	{
		theConnection() << headerUpgrade(request);
		return (new WebSocketHandle(*this));
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