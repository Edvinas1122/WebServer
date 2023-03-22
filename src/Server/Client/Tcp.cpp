#include <Client.hpp>

#define RECEIVE_BUFFER_SIZE 1024

bool	Tcp::receivePacket()
{
	char	mesg[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	bytes_read = recv(fd, mesg, RECEIVE_BUFFER_SIZE, 0);
	if (bytes_read == -1)
		throw std::exception();
	if (bytes_read > 0) {
		incoming.append(mesg, bytes_read);
		return (true);
	}
	return (false);
}

bool	Tcp::sendPacket()
{
	int		buffer_len = outgoing.length();
	int		bytes_sent;
	int		send_len;

	send_len = buffer_len;
	if (buffer_len > RECEIVE_BUFFER_SIZE)
		send_len = 1024;
	bytes_sent = send(fd, (void*)outgoing.substr(0, buffer_len).data(), send_len, MSG_NOSIGNAL);
	if (bytes_sent == -1)
		throw std::exception();
	if (bytes_sent > 0) {
		outgoing = outgoing.substr(bytes_sent, outgoing.length());
		return (true);
	}
	return (false);
}

bool	Tcp::uploadBufferNotEmpty() const
{
	if (!outgoing.empty()) // not tested for incoming
		return (true);
	return (false);
}


/*
	Stream overloads
*/

Tcp	&Tcp::operator<<(const std::string& str)
{
	outgoing.append(str, str.length());
	return (*this);
}

Tcp	&Tcp::operator<<(const char *str)
{
	outgoing.append(str, strlen(str));
	return (*this);
}

