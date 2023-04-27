#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

bool	WebSocketHandle::Handle()
{
	Buffer newstr;
	std::string rcv;

	rcv = DataFeed("WebSocket", who);
	if (!rcv.empty())
	{
		theConnection() << send_websocket_message(rcv);
		std::cout << rcv << std::endl;
	}
	theConnection() >> newstr;
	if (newstr.length()) {
		addSystemMessage("IRC", "messages", who + " " + receive_websocket_message((*newstr), newstr.length()));
		std::cout << receive_websocket_message((*newstr), newstr.length()) << std::endl;
		theConnection() << send_websocket_message("reveiv");
	}
	return (true);
}