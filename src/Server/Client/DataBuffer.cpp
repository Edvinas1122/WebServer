#include <Client.hpp>

bool	BufferQueController::sendPacket()
{
	if (outgoing.length())
		return (Tcp::sendPacket());
	else if (file.is_open() && !incoming_transmission) {
		if (file.peek() != std::ifstream::traits_type::eof()) {
			outgoing << file;
			return (Tcp::sendPacket());
		}
		else {
			file.close();
			return (Tcp::sendPacket());
		}
	}
	return (false);
}

bool	BufferQueController::receivePacket()
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

BufferQueController	&BufferQueController::operator<<(const File& src)
{
	file = src;
	file.Open();
	return (*this);
}


/*
	Packet is available for sending
*/
bool	BufferQueController::uploadBufferNotEmpty() const
{
	if (Tcp::uploadBufferNotEmpty() || (file.is_open() && !incoming_transmission)) // not tested for incoming
		return (true);
	return (false);
}