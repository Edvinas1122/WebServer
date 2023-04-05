#include <Terminal.hpp>

static void	display_information()
{
	std::cout << "Command list:\n 1. end - ends program" << std::endl;
}

static void display_assitant_info()
{
	std::cout << "Unknown input.\nType \"help\" for command list" << std::endl;
}

static void end_program()
{
	std::cout << "Quiting program" << std::endl;
}

bool	TerminalChat::parce_input(std::string const &user_input)
{
	if (!user_input.length())
		return (false);
	if (!user_input.compare(0, 4, "help"))
		display_information();
	if (!user_input.compare(0, 4, "kill"))
		throw std::exception();
	else if (!user_input.compare(0, 8, "continue"))
	{
		clearMessage();
		QueFollowingProcess(new TerminalChat(&theConnection()));
		std::cout << "process set to continue" << std::endl;
	}
	else if (!user_input.compare(0, 4, "send"))
	{
		QueFollowingProcess(new FileSend(&theConnection(), new TerminalChat(&theConnection()), "/home/WebServer/http/files/test.txt"));
		End();
	}
	else if (!user_input.compare(0, 3, "end"))
	{
		end_program();
		return (false);
	}
	else if (!user_input.compare(0, 3, "say"))
	{
		display_assitant_info();
	}
	else
		message.append(user_input);
	return (true);
}

bool TerminalChat::terminal_interface()
{
	char c;
	ssize_t bytes_read = 0;
	std::string	user_input;

	bytes_read = read(STDIN_FILENO, &c, 1);
	if (bytes_read > 0)
	{
		while (bytes_read > 0)
		{
			user_input += c;
			bytes_read = read(STDIN_FILENO, &c, 1);
			if (errno != EAGAIN)
				break;
		}
		if (user_input.length())
			return (parce_input(user_input));
	}
	return (true);
}

void	TerminalChat::clearMessage()
{
	message.clear();
}

std::string	TerminalChat::extractMessage() const
{
	return (message);
}

bool	TerminalChat::notEmpty() const
{
	if (message.length())
		return (true);
	return (false);
}

bool	TerminalChat::Handle()
{
	if (!terminal_interface())
		return (false);
	if (notEmpty()) {
		theConnection() << extractMessage();
		clearMessage();
		
		return (true);
	}
	return (true);
}

bool	Roast::Handle()
{
	theConnection() << "No you are!\r\n";
	return (false);
}

ServiceProcess	*Roast::NextProcess()
{
	return (new FileSend(&theConnection(), "/home/WebServer/http/files/test.txt"));
}

// ServiceProcess	*TerminalChat::NextProcess()
// {
// 	if (continuation)
// 		return (new TerminalChat(&theConnection()));
// 	return (NULL);
// }

ServiceProcess	*Terminal::RequestParse(Connection *connection, std::string const &request)
{
	if (request.find("Contact") != std::string::npos)
		return (new TerminalIntroduction(connection));
	if (request.find("You are gay") != std::string::npos)
		return (new Roast(connection));
	if (request.find("Gain") != std::string::npos)
		return (new FileSend(connection, std::string("/home/WebServer/http/files/") + request.substr(5, request.length() - 7)));
	if (request.find("HTTP") != std::string::npos)
		return (new HTTPParser(connection, NULL));
	return (NULL);
}

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

bool	TerminalParser::Handle()
{
	theConnection() >> request;
	if (!request.empty())
	{
		if (request.find("Gay") != std::string::npos)
		{
			QueFollowingProcess(new FileSend(&theConnection(), new TerminalIntroduction(&theConnection()), "/home/WebServer/http/files/test.txt"));
			return (false);
		}
		if (request.find("Chat") != std::string::npos)
		{
			QueFollowingProcess(new TerminalChat(&theConnection()));
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
			QueFollowingProcess(new FileSend(&theConnection(), new TerminalIntroduction(&theConnection()), std::string("/home/WebServer/http/files/") + request.substr(4, request.length() - 6)));
			return (false);
		}
		if (request.find("Post") != std::string::npos)
		{
			theConnection() << "Please write 4 characters..." << "\n";
			QueFollowingProcess(new FileReceive(&theConnection(), new TerminalIntroduction(&theConnection()), "/home/WebServer/http/files/utest.txt", 4));
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
	}
	return (true);
}