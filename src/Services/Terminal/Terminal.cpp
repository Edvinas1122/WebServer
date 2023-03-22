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

bool	Terminal::parce_input(std::string const &user_input)
{
	if (!user_input.length())
		return (false);
	if (!user_input.compare(0, 4, "help"))
		display_information();
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

bool Terminal::terminal_interface()
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

void	Terminal::clearMessage()
{
	message.clear();
}

std::string	Terminal::extractMessage()
{
	return (message);
}

bool	Terminal::notEmpty()
{
	if (message.length())
		return (true);
	return (false);
}

bool	Terminal::Ready(Client &client)
{
	(void) client;
	return (notEmpty());
};

void	Terminal::Serve(Client &client)
{
	client.setInactiveTimeOutCounter(14);
	client << extractMessage();
};

void	Terminal::Handle(Client &client)
{
	if (client.incomingAvailable())
	{
		if (client.getMessage().find("You are gay") != std::string::npos)
		{
			client << "No you are";
			client << File("/home/WebServer/http/files/test.txt");
			client.setInactiveTimeOutCounter(0);
			std::cout << client;
			return ;
		}
	}
	clearMessage();
};