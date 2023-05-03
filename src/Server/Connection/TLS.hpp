#ifndef TLC_HPP
# define TLC_HPP

# include <includes.hpp>
# include "TCP.hpp"

class	TLS: public TCP
{
	private:
		SSL *ssl;
	public:
		TLS(){};
		TLS(SSL *ssl, const int fd, struct sockaddr_in const &socketAddress, std::string const &port): TCP(fd, socketAddress, port), ssl(ssl) {};
		TLS(const TLS &src): TCP(src), ssl(src.ssl) {};
		virtual ~TLS();
		TLS	&operator=(const TLS &src)
		{
			if (this != &src) {
				TCP::operator=(src);
			}
			return (*this);
		};
		bool	sendPacket();
		bool	receivePacket();
};

#endif