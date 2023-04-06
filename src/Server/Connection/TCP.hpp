#ifndef TCP_HPP
# define TCP_HPP

# include <includes.hpp>
# include <Connection.hpp>

class	TCP: public Connection
{
	private:
		sockaddr_in			socketAddress;
		const std::string	port;
	public:
		TCP() {};
		TCP(const int fd, struct sockaddr_in const &socketAddress, std::string const &port): Connection(fd), socketAddress(socketAddress), port(port) {};
		TCP(const TCP &src): Connection(src), socketAddress(src.socketAddress), port(src.port) {};
		TCP(const int &fd): Connection(fd) {};
		TCP &operator=(const TCP &src)
		{
			if (this != &src) {
				Connection::operator=(src);
				socketAddress = src.socketAddress;
			}
			return (*this);
		};
		~TCP() {};

	bool		sendPacket();
	bool		receivePacket();

	bool operator==(const Connection& other) const {
		return (Connection::operator==(other));
	}
};

#endif