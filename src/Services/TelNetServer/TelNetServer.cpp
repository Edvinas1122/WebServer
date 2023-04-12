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
			SetFollowingProcess(new TelNetServerIntroduction(*this));
			return (false);
		}
		if (request.find("Chat") != std::string::npos)
		{
			theConnection() << "Chat started\ntype end to disconect\n";
			SetFollowingProcess(new TelNetServerChat(&theConnection(), new TelNetServerIntroduction(*this)));
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
			SetFollowingProcess(new FileSend(&theConnection(), new TelNetServerIntroduction(*this), std::string("/home/WebServer/http/files/") + request.substr(4, request.length() - 6)));
			return (false);
		}
		if (request.find("Post") != std::string::npos)
		{
			theConnection() << "Please write 4 characters..." << "\n";
			SetFollowingProcess(new FileReceive(&theConnection(), new TelNetServerIntroduction(*this), "/home/WebServer/http/files/utest.txt", 4));
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
		if (request.find("Bash") != std::string::npos)
		{
			if (request.find(" ") == std::string::npos)
				theConnection() << "Follow bash with command script";
			else
			{
				std::string	file = request.substr(request.find(" ") + 1);
				file = file.substr(0, file.length() - 2);
				SetFollowingProcess(new ExecuteFile(&theConnection(), new TelNetServerParser(&theConnection()), "/usr/bin/sh", file));
			}
			return (false);
		}
		if (request.find("Exec") != std::string::npos)
		{
			if (request.find(" ") == std::string::npos)
				theConnection() << "Exec [executable_path] [command]";
			else
			{
				std::string	file = request.substr(request.find(" ") + 1);
				std::string	command = request.substr(request.find_last_of(" ") + 1);
				file = file.substr(0, file.find_last_of(" "));
				command = command.substr(0, command.length() - 2);
				SetFollowingProcess(new ExecuteFile(&theConnection(), new TelNetServerParser(&theConnection()), file, command));
			}
			return (false);
		}
		if (request.find("Pipe") != std::string::npos)
		{
			if (request.find(" ") == std::string::npos)
				theConnection() << "Pipe [php script path] [post request]";
			else
			{
				std::string	script_path = request.substr(request.find(" ") + 1);
				std::string	post_request = request.substr(request.find_last_of(" ") + 1);

				script_path = script_path.substr(0, script_path.find_last_of(" "));
				post_request = post_request.substr(0, post_request.length() - 2);
				SetFollowingProcess(new ExecuteFile(&theConnection(), new TelNetServerParser(&theConnection()), script_path, post_request));
			}
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

bool	TelNetServerChat::Handle()
{
	if (theConnection().downloadBufferReady())
	{
		std::string	buf;
		theConnection() >> buf;
		if (!buf.empty())
		{
			if (buf.find("terminate") != std::string::npos)
			{
				std::cout << "Terminating" << std::endl;
				ProgramInterface::Terminate();
			}
			else if (buf.find("background") != std::string::npos)
			{
				addSystemMessage("Terminal", "command", "background");
				End();
				return (false);
			}
			else if (buf.find("print") != std::string::npos)
			{
				addSystemMessage("Terminal", "command", "print");
				End();
				return (false);
			}
			else if (buf.find("runtime") != std::string::npos)
			{
				addSystemMessage("Terminal", "command", "runtime");
				End();
				return (false);
			}
			else if (buf.find("file_pr") != std::string::npos)
			{
				addSystemMessage("Terminal", "command", "file_pr");
				End();
				return (false);
			}
			else if (buf.find("end") != std::string::npos)
			{
				End();
				return (false);
			}
			std::cout << buf << std::endl;
		}
	}
	theConnection() << ProgramInterface::DataFeed("terminal_chat", "message");
	return (true);
};