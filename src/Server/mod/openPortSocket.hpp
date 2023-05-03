#ifndef OPEN_PORT_SOCKET_HPP
# define OPEN_PORT_SOCKET_HPP

# include <includes.hpp>

# define SUCESS 0
# define FAILED_SOCKET -1
# define LIST_END NULL

# define MAX_REQUEST_QUE 10

class	BindFailure: public std::exception {};
class	SSLCertificateFailure: public std::exception {};

int	openPortSocket(char const *port_number) EXCEPTION;
SSL_CTX* create_ssl_context(const char *cert_file, const char *key_file) EXCEPTION;

#endif