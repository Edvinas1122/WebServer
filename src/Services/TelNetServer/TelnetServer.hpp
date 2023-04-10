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
};

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