#ifndef TERMINAL_HPP
# define TERMINAL_HPP

#include <includes.hpp>
#include <Service.hpp>

class	Terminal: public Service
{
	private:
		std::string	message;
	public:
		Terminal() {};

		virtual bool	Ready(Client &client) {
			(void) client;
			return (notEmpty());
		};

		virtual void	Serve(Client &client)
		{
			client.setInactiveTimeOutCounter(4);
			client << extractMessage();
		};

		virtual	void	Handle() {
			if (notEmpty()) {
				clearMessage();
			}
		};

		bool	terminal_interface();
	private:
		bool		notEmpty();
		std::string	extractMessage();
		void		clearMessage();
	private:
		bool		parce_input(std::string const &user_input);
};

#endif