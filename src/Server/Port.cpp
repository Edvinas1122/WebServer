#include "Server.hpp"

Connection	*Port::getConnection(const int fd, struct sockaddr_in &socketAddress)
{
	return (new TCP(fd, socketAddress, port));
};