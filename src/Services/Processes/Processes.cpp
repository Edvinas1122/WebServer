#include "Processes.hpp"

bool	FileSend::Handle()
{
	theConnection() << fileToSend;
	if (fileToSend.GetRemainingLen())
		return (true);
	std::cout << "send completed" << std::endl;
	return (false);
}

bool	FileReceive::Handle()
{
	Buffer	tst;

	theConnection() >> tst;
	if (!tst.empty())
		tst >> fileToReceive;
	if (lenght <= fileToReceive.GetSize())
	{
		std::cout << "file received" << std::endl;
		return (false);
	}
	return (true);
}


bool	MasterProcess::Handle()
{
	ServiceProcess	*followingProcess;

	if (!theConnection().getMessage().empty())
	{
		try {
			followingProcess = RequestParse(theConnection().getMessage());
		} catch (...)
		{
			std::cerr << "Parcing failure" << std::endl;
			throw (std::exception());
		}
		if (followingProcess != NULL)
		{
			QueFollowingProcess(followingProcess);
			return (false);
		}
		throw (std::exception()); // undefined syntax
	}
	return (true);
};

static std::string	typeInfo(std::string const &path)
{
	struct stat	info;

	if (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
		return ("Dir: ");
	else
		return ("File: ");
}

static std::string	dirInfo(const char *path)
{
	DIR				*dir_ptr;
	struct dirent	*entry;
	std::string		info;

	dir_ptr = opendir(path);
	if (dir_ptr == NULL)
		throw std::exception();
	while (42)
	{
		entry = readdir(dir_ptr);
		if (entry == NULL)
			break;
		info.append("\n");
		info.append(typeInfo(std::string(path) + std::string("/") + std::string(entry->d_name)));
		info.append(entry->d_name);
	}
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

static std::string	HTTPHeaderDirOK(std::string const &path)
{
	std::stringstream	header;

	header << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n";
	header << "Content-Length: " << dirInfo(path.c_str()).length() << "\r\n";
	header << "\r\n";
	header << dirInfo(path.c_str());
	return (header.str());
}

ServiceProcess	*HTTPParser::RequestParse(std::string const &request)
{
	if (HttpRequest(request).getMethod() == "GET")
	{
		std::string	dir = std::string("/home/WebServer/http") + HttpRequest(request).getLocation();

		if (HttpRequest(request).getLocation() == "/")
		{
			std::string	defaultResponse = "index.html";
			dir += defaultResponse;
		}
		if (!Access(dir))
		{
			theConnection() << "HTTP/1.1 404 Not Found\r\n\r\n";
			return (new HTTPParser(*this));
		}
		if (!isFile(dir))
		{
			theConnection() << HTTPHeaderDirOK(dir);
			return (new HTTPParser(*this));
		}
		theConnection() << HTTPHeaderFileOK(dir);
		return (new HTTPFileSend(*this, new HTTPParser(*this), dir));
	}
	return (new HTTPParser(*this));
};

bool	HTTPFileSend::Handle()
{
	if (theConnection().getMessage().empty())
		return (FileSend::Handle());
	else
	{
		HTTPParser::Handle();
		return (true);
	}
}

// ServiceProcess	*HTTPFileSend::RequestParse(const HttpRequest &request)
// {
//     return	(HTTPParser::RequestParse(request));
// }