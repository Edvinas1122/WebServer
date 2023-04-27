#ifndef TELNET_SERVER_HPP
# define TELNET_SERVER_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Connection.hpp>
# include <Processes.hpp>

class	TelNetServerParser : public ServiceProcess
{
	private:
		std::string	request;
	public:
		TelNetServerParser(ServiceProcess const &src): ServiceProcess(src) {};
		TelNetServerParser(Connection *connection): ServiceProcess(connection) {};
		virtual ~TelNetServerParser() {};

	bool	Handle();
};

class	TelNetServerIntroduction : public ServiceProcess
{
	public:
		TelNetServerIntroduction(Connection *connection, ServiceProcess *followingProcess): ServiceProcess(connection, followingProcess) {};
		TelNetServerIntroduction(TelNetServerIntroduction const &src): ServiceProcess(src, new TelNetServerParser(src)) {};
		TelNetServerIntroduction(ServiceProcess const &src): ServiceProcess(src, new TelNetServerParser(src)) {};
		virtual ~TelNetServerIntroduction() {};

	bool	Handle() {
		theConnection() << "Command list:\n 1. Gay\n 2. Chat\n 3. Get\n 4. Exit\n 5. Close\n 6. Post\n 7. Execute - Exec\n 8. Bash\n";
		return (false);
	};

	// bool	Handle() {
	// 	theConnection() << ":c2s15d82.42wolfsburg.de 001 a :Welcome to the Internet Relay Network a!a@127.0.0.1\r\n:c2s15d82.42wolfsburg.de 002 a :Your host is c2s15d82.42wolfsburg.de, running version ft_irc 1.0\r\n:c2s15d82.42wolfsburg.de 003 a :This server was created 2023-03-29\r\n:c2s15d82.42wolfsburg.de 004 a :c2s15d82.42wolfsburg.de ft_irc 1.0 io opsitnmlbvk\r\n:c2s15d82.42wolfsburg.de 375 a :- c2s15d82.42wolfsburg.de Message of the day - \r\n:c2s15d82.42wolfsburg.de 372 a :- It's a wonderful day!\r\n:c2s15d82.42wolfsburg.de 376 a :End of MOTD command\r\n";
	// 	// theConnection() << ":46.101.198.64 001 a :Command list\r\n";
	// 	return (false);
	// };
};

// ENCODING ASCII

// HOST 46.101.198.64:+10015
// NICK gaygay
// USER gaygay 0 * :gaygay

// :c2s15d82.42wolfsburg.de 001 a :Welcome to the Internet Relay Network a!a@127.0.0.1
// :c2s15d82.42wolfsburg.de 002 a :Your host is c2s15d82.42wolfsburg.de, running version ft_irc 1.0
// :c2s15d82.42wolfsburg.de 003 a :This server was created 2023-03-29
// :c2s15d82.42wolfsburg.de 004 a :c2s15d82.42wolfsburg.de ft_irc 1.0 io opsitnmlbvk
// :c2s15d82.42wolfsburg.de 375 a :- c2s15d82.42wolfsburg.de Message of the day - 
// :c2s15d82.42wolfsburg.de 372 a :- It's a wonderful day!
// :c2s15d82.42wolfsburg.de 376 a :End of MOTD command

#include <ProgramInterface.hpp>

class	TelNetServerChat: public ServiceProcess, public Terminal
{
	public:
		TelNetServerChat(Connection *connection): ServiceProcess(connection) {};
		TelNetServerChat(Connection *connection, ServiceProcess *followingService):
							ServiceProcess(connection, followingService) {};
		~TelNetServerChat() {};
	
	bool	Handle();
};

class	TelNetServer: public Service
{
	public:
		TelNetServer() {};
		~TelNetServer() {};

	ServiceProcess	*RequestParse(Connection *connection, std::string const &request);
	size_t	TimeOutAge() const {return (60);};

};

#endif