#include "Server.hpp"

Connection	*Port::getConnection(const int fd, struct sockaddr_in &socketAddress)
{
	return (new TCP(fd, socketAddress, port));
};

#include <TLS.hpp>

Connection	*SSLPort::getConnection(const int fd, struct sockaddr_in &socketAddress) EXCEPTION
{
	SSL *ssl = SSL_new(certificate);

	if (!ssl)
	{
		fprintf(stderr, "SSL_new() failed\n");
		ERR_print_errors_fp(stderr);
		throw std::exception();
	}
	SSL_set_fd(ssl, fd);
	if (SSL_accept(ssl) <= 0)
	{
		ERR_print_errors_fp(stderr);
		SSL_shutdown(ssl);
    	SSL_free(ssl);
		throw ConnectFailure();
	}
	return (new TLS(ssl, fd, socketAddress, port));
};

SSLPort::~SSLPort()
{
	SSL_CTX_free(certificate);
};