#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Connection.hpp>
# include <Processes.hpp>

class	IRCParser: public ServiceProcess, public Terminal
{
	private:
		std::string	request;
	public:
		IRCParser(ServiceProcess const &src): ServiceProcess(src), loudSpeak(false) {};
		IRCParser(Connection *connection): ServiceProcess(connection), loudSpeak(false) {};
		virtual ~IRCParser() {};

	bool	Handle();
	private:
	void	parseBot(std::string const &request);
	bool		loudSpeak;
	std::string who;
};

std::string irc_server_response(const std::string& client_hostname)
{
    std::string response = ":" + client_hostname + " NOTICE AUTH :*** Looking up your hostname...\r\n" +
                        ":" + client_hostname + " NOTICE AUTH :*** Found your hostname (edvinas)\r\n" +
                        ":" + client_hostname + " 001 edvinas :Welcome to the IRC server edvinas! Your host is " + client_hostname + ", running version 2.0\r\n" +
                        ":" + client_hostname + " 002 edvinas :Your connection to the server has been established successfully\r\n" +
                        ":" + client_hostname + " 003 edvinas :This server was created on Mon Apr 25 2023 at 12:00:00 UTC\r\n" +
                        ":" + client_hostname + " 004 edvinas " + client_hostname + " 2.0 a b\r\n" +
                        ":" + client_hostname + " 375 edvinas :- " + client_hostname + " Message of the Day -\r\n" +
                        ":" + client_hostname + " 372 edvinas :- Welcome to the IRC server edvinas! Enjoy your stay.\r\n" +
                        ":" + client_hostname + " 376 edvinas :End of /MOTD command.\r\n";
    
    return (response);
}

std::string irc_list_response(const std::string& client_hostname)
{
	std::string response = ":" + client_hostname + " 321 edvinas Channel :Users  Name\r\n"
						":" + client_hostname + " 322 edvinas #example 10 :\r\n"
						":" + client_hostname + " 323 edvinas :End of /LIST\r\n";
	
	return (response);
}

std::string irc_join_response(const std::string& client_hostname, const std::string& channel_name)
{
    std::string response = ":" + client_hostname + " 332 edvinas " + channel_name + " :\r\n"
                           ":" + client_hostname + " 353 edvinas = " + channel_name + " :edvinas armin\r\n"
                           ":" + client_hostname + " 366 edvinas " + channel_name + " :End of /NAMES list\r\n"
						    ":" + client_hostname + " JOIN " + channel_name + "\r\n";
    
    return (response);
}

std::string irc_who_response(const std::string& client_hostname, const std::string& channel_name)
{
    std::string response = ":" + client_hostname + " 352 edvinas " + channel_name + " armin " + client_hostname + " armin H :0 armin\r\n"
							":" + client_hostname + " 352 edvinas " + channel_name + " ~edvinas 46.101.198.64 " + client_hostname + " edvinas H :0 edvinas\r\n"
                           ":" + client_hostname + " 315 edvinas " + channel_name + " :End of /WHO list\r\n";
    
    return response;
}

static std::string	logoGee()
{
	return("        __\n     __/o \\_\n     \\____  \\\n         /   \\\n   __   //\\   \\\n__/o \\-//--\\   \\_/\n\\____  ___  \\  |\n     ||   \\ |\\ |\n    _||   _||_||\n");
}

std::string irc_message_split(const std::string& sender, const std::string& channel, const std::string& message)
{
    std::string response;
    std::string::size_type start = 0;
    std::string::size_type end = message.find('\n');

    while (end != std::string::npos) {
        std::string line = message.substr(start, end - start);
        response += ":" + sender + " PRIVMSG " + channel + " :" + line + "\r\n";
        start = end + 1;
        end = message.find('\n', start);
    }

    if (start < message.size()) {
        std::string line = message.substr(start);
        response += ":" + sender + " PRIVMSG " + channel + " :" + line + "\r\n";
    }

    return response;
}

std::string irc_message(const std::string& sender, const std::string& channel, const std::string& message)
{
    std::string response = ":" + sender + " PRIVMSG " + channel + " :" + message + "\r\n";
    return response;
}


void	IRCParser::parseBot(const std::string &request)
{
	std::cout << request << std::endl;
	if (request.find("terminate") != std::string::npos)
		Terminate();
	else if (request.find("Gay") != std::string::npos)
		theConnection() << irc_message_split("armin", "#example", logoGee());
	else if (request.find("background") != std::string::npos)
		addSystemMessage("Terminal", "command", "background");
	else if (request.find("info") != std::string::npos)
	{
		addSystemMessage("Terminal", "ConnectionInfo", "get");
	}
	else if (request.find("runtime") != std::string::npos)
	{
		addSystemMessage("Terminal", "command", "runtime");
	}
	else if (request.find("log") != std::string::npos)
	{
		addSystemMessage("Terminal", "command", "file_pr");
	}
	else if (request.find("print") != std::string::npos)
	{
		addSystemMessage("Terminal", "command", "print");
	}
	else if (request.find("say ") != std::string::npos)
	{
		loudSpeak ? loudSpeak = false : loudSpeak = true;
		size_t	begin = request.find("say ") + 4;
		size_t	end = request.find("\r\n", begin) - begin;
		who = request.substr(begin, end);
		if (loudSpeak)
			theConnection() << irc_message("armin", "#example", std::string("speaking to ") + who);
	}
	else if (loudSpeak)
	{
		// size_t	begin = request.find("say ") + 4;
		size_t	begin = 1;
		size_t	end = request.find("\r\n", begin) - begin;
		addSystemMessage("WebSocket", who, request.substr(begin, end));
	}
	else
	{
		theConnection() << irc_message_split("armin", "#example", "Command list:\n 1. Gay\n 2. terminate\n 3. background\n 4. info\n 5. runtime - dissable/enable terminal print runtime info\n 6. print - disable/enable server request print\n 7. log - out to log file\n 8 say[space] - enable online speak");
		return;
	}
	theConnection() << irc_message("armin", "#example", "Interpretated!");
}

bool	IRCParser::Handle()
{
	theConnection() >> request;
	if (request.find("USER") != std::string::npos)
	{
		theConnection() << irc_server_response("edvinas!user@host.com");
		request.clear();
	}
	else if (request.find("LIST") != std::string::npos)
	{
		theConnection() << irc_list_response("edvinas!user@host.com");
		request.clear();
	}
	else if (request.find("JOIN") != std::string::npos)
	{
		theConnection() << irc_join_response("edvinas!user@host.com", "#example");
		request.clear();
	}
	else if (request.find("WHO") != std::string::npos)
	{
		theConnection() << irc_who_response("edvinas!user@host.com", "#example");
		request.clear();
	}
	else if (request.find("PING") != std::string::npos)
	{
		size_t	begin = request.find("PING") + 6;
		size_t	end = request.find("\r\n", begin) - begin;
		std::cout << request.substr(begin, end);
		theConnection() << "PONG :" << request.substr(begin, end) << "\r\n";
		request.clear();
	}
	else if (request.find("PRIVMSG #example") != std::string::npos)
	{
		size_t	begin = request.find("PRIVMSG") + 17;
		size_t	end = request.find("\r\n", begin) - begin;

		parseBot(request.substr(begin, end));
		request.clear();
	}
	theConnection() << irc_message_split("armin", "#example", DataFeed("IRC", "messages"));
	return (true);
};

class	IRCService: public Service
{
	public:
		IRCService() {};
		~IRCService() {};

	ServiceProcess	*RequestParse(Connection *connection, std::string const &request);
	size_t	TimeOutAge() const {return (600);};

};

ServiceProcess	*IRCService::RequestParse(Connection *connection, std::string const &request)
{
	if (request.find("NICK edvinas") != std::string::npos)
		return (new IRCParser(connection));
	return (NULL);
};

#endif
