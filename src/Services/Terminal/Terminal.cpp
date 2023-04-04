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

bool	TerminalResponse::parce_input(std::string const &user_input)
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
		continuation = true;
		std::cout << "process set to continue" << std::endl;
	}
	else if (!user_input.compare(0, 4, "send"))
	{
		QueFollowingProcess(new FileSend(&theConnection(), new TerminalResponse(&theConnection()), "/home/WebServer/http/files/test.txt"));
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

bool TerminalResponse::terminal_interface()
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

void	TerminalResponse::clearMessage()
{
	message.clear();
}

std::string	TerminalResponse::extractMessage() const
{
	return (message);
}

bool	TerminalResponse::notEmpty() const
{
	if (message.length())
		return (true);
	return (false);
}

bool	TerminalResponse::Handle()
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

// ServiceProcess	*TerminalResponse::NextProcess()
// {
// 	if (continuation)
// 		return (new TerminalResponse(&theConnection()));
// 	return (NULL);
// }

ServiceProcess	*Terminal::RequestParse(Connection *connection, std::string const &request)
{
	if (request.find("Contact") != std::string::npos)
		return (new TerminalResponse(connection));
	if (request.find("You are gay") != std::string::npos)
		return (new Roast(connection));
	if (request.find("Gain") != std::string::npos)
		return (new FileSend(connection, std::string("/home/WebServer/http/files/") + request.substr(5, request.length() - 7)));
	return (NULL);
}

// bool	Terminal::Ready(Client &client)
// {
// 	(void) client;
// 	return (notEmpty());
// };



// void	Terminal::SetResponse(Client &client)
// {
// 	client.setInactiveTimeOutCounter(14);
// 	client << extractMessage();
// 	if (client.incomingAvailable())
// 	{
// 		if (client.getMessage().find("You are gay") != std::string::npos)
// 		{
// 			client << "No you are";
// 			client << File("/home/WebServer/http/files/test.txt");
// 			client.setInactiveTimeOutCounter(0);
// 			std::cout << client;
// 			return ;
// 		}
// 	}
// };

// void	Terminal::Handle(ServiceProcess *process)
// {
// 	(void) process;
// 	clearMessage();
// };