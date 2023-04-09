#include "TelnetServer.hpp"

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
	closedir(dir_ptr);
	return (info);
}

static std::string	logoGee()
{
	return("        __\n     __/o \\_\n     \\____  \\\n         /   \\\n   __   //\\   \\\n__/o \\-//--\\   \\_/\n\\____  ___  \\  |\n     ||   \\ |\\ |\n    _||   _||_||\n");
}

bool	TelNetServerParser::Handle()
{
	theConnection() >> request;
	if (!request.empty())
	{
		if (request.find("Gay") != std::string::npos)
		{
			theConnection() << logoGee();
			QueFollowingProcess(new TelNetServerIntroduction(*this));
			return (false);
		}
		if (request.find("Chat") != std::string::npos)
		{
			theConnection() << "Chat started\ntype end to disconect\n";
			QueFollowingProcess(new TelNetServerChat(&theConnection(), new TelNetServerIntroduction(*this)));
			return (false);
		}
		if (request.find("Get") != std::string::npos)
		{
			if (request.length() == 5)
			{
				theConnection() <<  dirInfo("/home/WebServer/http/files/") << "\n";
				request.clear();
				return (true);
			}
			QueFollowingProcess(new FileSend(&theConnection(), new TelNetServerIntroduction(*this), std::string("/home/WebServer/http/files/") + request.substr(4, request.length() - 6)));
			return (false);
		}
		if (request.find("Post") != std::string::npos)
		{
			theConnection() << "Please write 4 characters..." << "\n";
			QueFollowingProcess(new FileReceive(&theConnection(), new TelNetServerIntroduction(*this), "/home/WebServer/http/files/utest.txt", 4));
			return (false);
		}
		if (request.find("Close") != std::string::npos)
		{
			End();
			return (false);
		}
		if (request.find("Exit") != std::string::npos)
		{
			throw	std::exception();
		}
		if (request.find("Exec") != std::string::npos)
		{
			QueFollowingProcess(new ExecuteFile(&theConnection(), new TelNetServerIntroduction(*this), "/usr/bin/cat"));
			return (false);
		}
	}
	return (true);
}


ServiceProcess	*TelNetServer::RequestParse(Connection *connection, std::string const &request)
{
	if (request.find("gaygay") != std::string::npos)
		return (new TelNetServerIntroduction(connection, new TelNetServerParser(connection)));
	return (NULL);
}