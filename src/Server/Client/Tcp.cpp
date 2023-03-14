#include <Client.hpp>

#define RECEIVE_BUFFER_SIZE 1024

bool	Tcp::receivePacket()
{
	char	mesg[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	bytes_read = recv(fd, mesg, RECEIVE_BUFFER_SIZE, 0);
	if (bytes_read > 0) {
		incoming.append(mesg);
		return (true);
	}
	return (false);
}

bool	Tcp::sendPacket()
{
	std::string	buffer = outgoing.substr(0, 1024);
	int	bytes_sent;

	bytes_sent = send(fd, buffer.c_str(), buffer.length(), 0);
	if (bytes_sent == -1)
		throw std::exception();
	if (bytes_sent > 0) {
		outgoing = outgoing.substr(bytes_sent, outgoing.length());
		return (true);
	}
	return (false);
}

bool	Tcp::ready() const
{
	if (outgoing.length())
		return (true);
	return (false);
}

Tcp	&Tcp::operator<<(const std::string& str)
{
	outgoing.append(str);
	return (*this);
}

