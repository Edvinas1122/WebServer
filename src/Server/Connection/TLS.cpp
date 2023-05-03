#include "TLS.hpp"

bool	TLS::receivePacket()
{
	char	mesg[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	bytes_read = SSL_read(ssl, mesg, RECEIVE_BUFFER_SIZE);
	if (bytes_read == -1)
		throw std::exception();
	if (bytes_read > 0) {
		incoming.append(mesg, bytes_read);
		return (true);
	}
	return (false);
}

bool	TLS::sendPacket()
{
	int	buffer_len = outgoing.length();
	int	bytes_sent;
	int	send_len;

	send_len = buffer_len;
	if (buffer_len > RECEIVE_BUFFER_SIZE)
		send_len = 1024;
	bytes_sent = SSL_write(ssl, (void*)outgoing.substr(0, buffer_len).data(), send_len);
	if (bytes_sent == -1)
		throw std::exception();
	if (bytes_sent > 0) {
		outgoing = outgoing.substr(bytes_sent, outgoing.length() - bytes_sent);
		return (true);
	}
	return (false);
}

TLS::~TLS()
{
    SSL_shutdown(ssl);
    SSL_free(ssl);
}