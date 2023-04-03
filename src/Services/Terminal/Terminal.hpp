#ifndef TERMINAL_HPP
# define TERMINAL_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Connection.hpp>

class	TerminalResponse : public ServiceProcess
{
	private:
		std::string	message;
	public:
		TerminalResponse(Connection *connection): ServiceProcess(connection) {};
		virtual ~TerminalResponse() {};

	bool	Handle();

	private:

	bool		terminal_interface();
	bool		parce_input(std::string const &user_input);
	bool		notEmpty() const;
	std::string	extractMessage() const;
	void		clearMessage();
};

class	Terminal: public Service
{
	public:
		Terminal() {};
		~Terminal() {};

	ServiceProcess	*RequestParse(Connection *connection, std::string const &request);

};

#endif