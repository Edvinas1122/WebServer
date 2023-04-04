#ifndef TERMINAL_HPP
# define TERMINAL_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Connection.hpp>

class	TerminalResponse : public ServiceProcess
{
	private:
		bool		continuation;
		std::string	message;
	public:
		TerminalResponse(Connection *connection): ServiceProcess(connection), continuation(false) {};
		virtual ~TerminalResponse() {};

	bool	Handle();
	// virtual ServiceProcess	*NextProcess();

	private:

	bool		terminal_interface();
	bool		parce_input(std::string const &user_input);
	bool		notEmpty() const;
	std::string	extractMessage() const;
	void		clearMessage();
};

#include <Processes.hpp>

class	Roast : public ServiceProcess
{
	private:
		File	fileToSend;
	public:
		Roast(Connection *connection): ServiceProcess(connection) {};
		virtual ~Roast() {};

	bool			Handle();
	ServiceProcess	*NextProcess();
};


class	Terminal: public Service
{
	public:
		Terminal() {};
		~Terminal() {};

	ServiceProcess	*RequestParse(Connection *connection, std::string const &request);

};

#endif