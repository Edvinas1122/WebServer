#include "Connection.hpp"

const std::string	Connection::getMessage() const {
	return (incoming);
};

// int	Connection::FileDescriptor() const
// {
// 	return (fd);
// };

bool	Connection::uploadBufferReady() const
{
	return (!outgoing.empty());
}

bool	Connection::downloadBufferReady() const
{
	return (!incoming.empty());
}

Connection	&Connection::operator<<(const std::string& str)
{
	outgoing.append(str, str.length());
	return (*this);
}

Connection	&Connection::operator<<(const char *str)
{
	outgoing.append(str, strlen(str));
	return (*this);
}

std::ostream&	operator<<(std::ostream& os, Connection& obj)
{
	if (obj.incoming.length())
	{
		os << obj.incoming;
		obj.flushIncoming();
	}
	return (os);
};