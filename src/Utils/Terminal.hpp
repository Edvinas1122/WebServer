#ifndef TERMINAL_HPP
# define TERMINAL_HPP

#include <includes.hpp>

class	Terminal
{
	private:
		std::string	message;
	public:
		Terminal()
		{
			int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
			fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
		};
		bool	terminal_interface();
		bool	notEmpty();
		std::string	extractMessage();
		void	clearMessage();
	private:
		bool	parce_input(std::string const &user_input);
};

#endif