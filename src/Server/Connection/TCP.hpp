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

	const std::string	getPort() const {return (port);};

	bool operator==(const Connection& other) const {
		return (Connection::operator==(other));
	}
};


typedef	int	fd_t;
struct TCPConnectionOrigin {
	fd_t		fd;
	in_addr		ipAddress;
	std::string	port_number;
	TCPConnectionOrigin(const fd_t &fd, const in_addr &ipAddress, const std::string &port_number):
	fd(fd), ipAddress(ipAddress), port_number(port_number) {};
};

bool	operator<(const TCPConnectionOrigin& left, const TCPConnectionOrigin& right);

struct findByFD
{
	fd_t	fd;
	findByFD(const fd_t& fd) : fd(fd) {}
	bool operator()(const std::pair<TCPConnectionOrigin, TCP> &pair) const
	{
		return (pair.first.fd == fd);
	};
};

struct findByAddr
{
	in_addr	ipAddress;
	findByAddr(const in_addr& ipAddress) : ipAddress(ipAddress) {}
	bool operator()(const std::pair<TCPConnectionOrigin, TCP> &pair) const
	{
		return (pair.first.ipAddress.s_addr == ipAddress.s_addr);
	};
};


#endif