#include <Client.hpp>

bool	DataBuffer::sendPacket()
{
	if (outgoing.length())
		return (Tcp::sendPacket());
	else if (file.is_open() && !incoming_transmission) {
		if (file.peek() != std::ifstream::traits_type::eof()) {
			*this << file.GetContentsBuffer();
			return (Tcp::sendPacket());
		}
		else {
			file.close();
			return (Tcp::sendPacket());
		}
	}
	return (false);
}

bool	DataBuffer::receivePacket()
{
	if (incoming_transmission && file.is_open()) {
		if (!incoming.empty()) {
			file << incoming;
			// incoming.clear() << -- HAS TO BE VIPED
			return (Tcp::receivePacket());
		}
		else {
			file.close();
			return (Tcp::receivePacket());
		}
	}
	if (incoming.empty())
		return (Tcp::receivePacket());
	return (false);
}

DataBuffer	&DataBuffer::operator<<(const File& src)
{
	file = src;
	file.Open();
	return (*this);
}



bool	DataBuffer::ready() const
{
	if (Tcp::ready() || file.is_open()) // not tested for incoming
		return (true);
	return (false);
}