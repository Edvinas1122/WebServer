#include "Connection.hpp"

const std::string	Connection::getMessage() const {
	return (incoming);
};

// int	Connection::FileDescriptor() const
// {
// 	return (fd);
// };

void	Connection::sendBytes(void *buf, size_t len)
{
	outgoing.append((char *)buf, len);
}

bool	Connection::uploadBufferReady() const // inverted logic ???
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

Connection	&Connection::operator<<(const size_t &number)
{
	std::stringstream	ss;

    ss << number;
	outgoing.append(ss.str(), ss.str().length());
	return (*this);
}

Connection	&Connection::operator<<(const char *str)
{
	outgoing.append(str, strlen(str));
	return (*this);
}

Connection	&Connection::operator<<(File& src)
{
	this->outgoing << src;
	return (*this);
}

std::ostream&	operator<<(std::ostream& os, Connection& obj)
{
	if (obj.incoming.length())
		os << obj.incoming;
	return (os);
};