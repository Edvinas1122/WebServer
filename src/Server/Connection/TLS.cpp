#include "TLS.hpp"

bool	TLS::receivePacket()
{
	char	mesg[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;
	int		ssl_err_code = 0;

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	SSL_set_read_ahead(ssl, 0);
	bytes_read = SSL_read(ssl, mesg, RECEIVE_BUFFER_SIZE);
	if (bytes_read == 0)
		return (false);
	if (bytes_read == -1)
	{
		ssl_err_code = SSL_get_error(ssl, bytes_read);
		if (ssl_err_code == SSL_ERROR_WANT_READ)
			return (true);
		throw std::exception();
	}
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
	int err;

	send_len = buffer_len;
	if (buffer_len > RECEIVE_BUFFER_SIZE)
		send_len = 1024;
	bytes_sent = SSL_write(ssl, (void*)outgoing.substr(0, buffer_len).data(), send_len);
	if (bytes_sent == 0)
		return (false);
	if (bytes_sent == -1)
	{
		err = SSL_get_error(ssl, bytes_sent);
		if (err == SSL_ERROR_WANT_WRITE) {
			return (true);
		} else {
			throw std::exception();
		}
	}
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

bool	TLS::handShake()
{
	int	err;
	int	ssl_err_code;

	if (establishedHandshake)
		return (false);
	err = SSL_accept(ssl);
	if (err == 0) {
		ERR_print_errors_fp(stderr);
		throw std::exception();
	} else if (err < 0) {
		ssl_err_code = SSL_get_error(ssl, err);
		if (ssl_err_code == SSL_ERROR_WANT_READ || ssl_err_code == SSL_ERROR_WANT_WRITE)
			return (true);
		else
			throw std::exception();
	}
	establishedHandshake = true;
	return (false);
}