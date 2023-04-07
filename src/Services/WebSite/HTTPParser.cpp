#include "WebSite.hpp"

static std::string	typeInfo(std::string const &path)
{
	struct stat	info;

	if (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
		return ("Dir: ");
	else
		return ("File: ");
}

static std::string	UploadForm(std::string const &path)
{
	std::string	form;

	form.append("<div><form action=\"");
	form.append(path);
	if (path.at(path.length() - 1) != '/')
		form.append("/");
	form.append("\" method=\"post\" enctype=\"multipart/form-data\">");
	form.append("<text>Upload</text>");
	form.append("<input type=\"file\" name=\"fileToUpload\">");
	form.append("<input type=\"submit\" value=\"Upload\" name=\"submit\">");
	form.append("</form></div>");
	return (form);
}

static std::string	dirInfoHTTPFormat(const char *path, std::string const &url, bool displayUpload)
{
	DIR				*dir_ptr;
	struct dirent	*entry;
	std::string		info;

	dir_ptr = opendir(path);
	if (dir_ptr == NULL)
		throw std::exception();
	info.append("<head></head>");
	info.append("<body><div>");
	while (42)
	{
		entry = readdir(dir_ptr);
		if (entry == NULL)
			break;
		info.append("<div><a href=\"");
		info.append(url);
		if (url.at(url.length() - 1) != '/')
		{
			info.append("/");
		}
		info.append(entry->d_name);
		info.append("\">");
		info.append(typeInfo(std::string(path) + std::string("/") + std::string(entry->d_name)));
		info.append(entry->d_name);
		info.append("</a></div>");
	}
	info.append("</div>");
	if (displayUpload)
		info.append(UploadForm(url.c_str()));
	info.append("</body>");
	closedir(dir_ptr);
	return (info);
}

static bool	isFile(const std::string &path)
{
	struct stat path_stat;

	if (stat(path.c_str(), &path_stat) != 0) {
		// Error accessing path
		return false;
	}
	if (S_ISDIR(path_stat.st_mode)) {
		return false;
	} else {
		return true;
	}	
}

static bool	Access(const std::string &path)
{
	struct stat path_stat;

	if (stat(path.c_str(), &path_stat) != 0)
		return (false);
	return (true);
}

static std::string	HTTPHeaderFileOK(std::string const &path)
{
	std::stringstream	header;

	header << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n";
	header << "Content-Length: " << File(path.c_str()).GetSize() << "\r\n";
	header << "\r\n";
	return (header.str());
}

static std::string	HTTPHeaderDirOK(std::string const &path, const std::string &url)
{
	std::stringstream	header;

	header << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n";
	header << "Content-Length: " << dirInfoHTTPFormat(path.c_str(), url.c_str(), true).length() << "\r\n";
	header << "\r\n";
	header << dirInfoHTTPFormat(path.c_str(), url.c_str(), true);
	return (header.str());
}

bool	allowedDir(std::string const &path)
{
	(void) path;
	return (true);
}

static void	trimUntilFileBegin(Buffer &buffer)
{
	buffer = buffer.substr(buffer.find("Content-Type:"));
	buffer = buffer.substr(buffer.find("\r\n") + 4);
};

static Buffer	unchunkBegining(Buffer buffer, std::string const &delimeter)
{
	size_t	begin;

	begin = buffer.find(delimeter);
	if (begin != std::numeric_limits<size_t>::max())
		buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());
	begin = buffer.find(delimeter);
	if (begin != std::numeric_limits<size_t>::max())
		buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());
	return (buffer);
};


# define IS_CGI false
# define CGI NULL
# define DIR_LISTING_ALLOWED true
# define TEST_LOCATION_DEFAULT_RESPONSES(x) ( x == "/home/WebServer/http/")

ServiceProcess	*HTTPParser::RequestParse(std::string const &request)
{
	std::string	dir = std::string("/home/WebServer/http") + HttpRequest(request).getLocation();
	// std::string dir = GET_DIR_FROM_(request.PRORT, request.LOCATION);
	// TEST_FOR_PROXY_DIRS_(dir)
		// dir modify 
	if (HttpRequest(request).getMethod() == "GET")
	{
		if (!Access(dir)) {
			theConnection() << "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
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
			size_t	fileSize = atoi((HttpRequest(request).getHeaders().at("Content-Length").c_str()));
			HTTPFileReceive	*process;
			Buffer	TrimRequestHead;

			theConnection() >> TrimRequestHead;
			fileSize -= unchunkBegining(TrimRequestHead, HttpRequest(request).getBoundry()).length() - 26;
			std::cout << "File receive size: " << fileSize << std::endl;
			process = new HTTPFileReceive(*this, new HTTPFileReceiveReport(*this, new HTTPParser(*this)),
							dir + HttpRequest(request).getFilename(), fileSize, HttpRequest(request).getBoundry());
			TrimRequestHead = unchunkBegining(TrimRequestHead, HttpRequest(request).getBoundry());
			trimUntilFileBegin(TrimRequestHead);
			*process << TrimRequestHead;
			return (process);
		}
	}
	return (new TerminateProcess(&theConnection()));
};

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