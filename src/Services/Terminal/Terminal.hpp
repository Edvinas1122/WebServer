#ifndef TERMINAL_HPP
# define TERMINAL_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Connection.hpp>
# include <Processes.hpp>


class	TerminalChat : public ServiceProcess
{
	private:
		std::string	message;
	public:
		TerminalChat(Connection *connection): ServiceProcess(connection) {};
		TerminalChat(Connection *connection, ServiceProcess *followingProcess):
							ServiceProcess(connection, followingProcess) {};
		virtual ~TerminalChat() {};

	bool	Handle();

	private:

	bool		terminal_interface();
	bool		parce_input(std::string const &user_input);
	bool		notEmpty() const;
	std::string	extractMessage() const;
	void		clearMessage();
};

class	TerminalParser : public ServiceProcess
{
	private:
		std::string	request;
	public:
		TerminalParser(Connection *connection): ServiceProcess(connection) {};
		virtual ~TerminalParser() {};

	bool	Handle();
};

class	TerminalIntroduction : public ServiceProcess
{
	public:
		TerminalIntroduction(Connection *connection): ServiceProcess(connection, new TerminalParser(connection)) {};
		virtual ~TerminalIntroduction() {};

	bool	Handle() {
		theConnection() << "Command list:\n 1. Gay\n 2. Chat\n 3. Get\n 4. Exit\n 5. Close\n 6. Post\n";
		return (false);
	};
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