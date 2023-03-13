// #include <ServerClasses.hpp>

// template<typename TYPE>
// Server<TYPE>::Run()
// {
// 	Observer::Poll(true);

// 	ClientsQue::setClients(getLoudSockets());

// 	ClientsQue::queProcess(pullIncoming, POLLIN);
// 	// ClientsQue::action(receivingAction);
// 	// ClientsQue::action(buildResponse);
// 	// terminal_interface();
// 	// ClientsQue::action(respondingAction, extractMessage());
// 	// clearMessage();

// 	ClientsQue::queProcess(pushOutgoing, POLLOUT);
// 	ClientsQue::closeTimeOut();
// }

// template<typename TYPE>
// bool	Server<TYPE>::pullIncoming(Client &client)
// {
// 	client.receivePacket();
// 	client.serviceStatus = false;
// 	return (true);
// }

// #define	CLOSE_CLIENT true

// template<typename TYPE>
// bool	Server<TYPE>::pushOutgoing(Client &client)
// {
// 	if (client.ready())
// 	{
// 		try {
// 			client.sendPacket();
// 			return (true);
// 		} catch(const std::exception& e) {
// 			client.updateTime(CLOSE_CLIENT);
// 			return (false);
// 		}	
// 	}
// 	return (false);
// }