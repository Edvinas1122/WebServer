#include "ProgramInterface.hpp"

std::string	ProgramInterface::data;
bool		ProgramInterface::run = true;

static void	display_information()
{
	std::cout << "Command list:\n 1. end - ends program\n 2. say - message telnet service clients\n" << std::endl;
}

static void display_assitant_info()
{
	std::cout << "Unknown input.\nType \"help\" for command list" << std::endl;
}

static void end_program_message()
{
	std::cout << "Quiting program" << std::endl;
}

void	Terminal::Parse(std::string const &user_input)
{
	if (!user_input.compare(0, 4, "help"))
		display_information();
	else if (!user_input.compare(0, 3, "say"))
		data.append(user_input.substr(4));
	else if (!user_input.compare(0, 3, "end"))
	{
		end_program_message();
		run = false;
	}
	else
		display_assitant_info();
}

void Terminal::Input()
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
			Parse(user_input);
	}
}

// void	TerminalChat::clearMessage()
// {
// 	message.clear();
// }

// std::string	TerminalChat::extractMessage() const
// {
// 	return (message);
// }

// bool	TerminalChat::notEmpty() const
// {
// 	if (message.length())
// 		return (true);
// 	return (false);
// }

// bool	TerminalChat::Handle()
// {
// 	if (!terminal_interface())
// 		return (false);
// 	if (notEmpty()) {
// 		theConnection() << extractMessage();
// 		clearMessage();
		
// 		return (true);
// 	}
// 	return (true);
// }
