#include <Client.hpp>

bool	BufferQueController::sendPacket()
{
	if (outgoing.length())
		return (Tcp::sendPacket());
	else if (file.is_open() && !incoming_transmission) {
		if (file.peek() != std::ifstream::traits_type::eof()) {
			outgoing << file;
			try {
				return (Tcp::sendPacket());
			} catch (...) {
				file.close();
				throw std::exception();
				return (false);
			}
		}
		else {
			file.close();
			setFlags(FILE_TRANSFERED);
			return (Tcp::sendPacket());
		}
	}
	return (false);
}

bool	BufferQueController::receivePacket()
{
	bool	receive_info = true;

	try {
		if (incoming.empty())
			receive_info = Tcp::receivePacket();
		if (receive_info && incoming_transmission && file.is_open()) {
			std::cout << "file incoming" << std::endl;
			file << incoming;
			incoming.clear();
		}
		return (receive_info);
	} catch (...) {
		if (incoming_transmission && file.is_open()) {
			file.close();
			setFlags(FILE_TRANSFERED);
		}
		return (false);
	}
}

// BufferQueController	&BufferQueController::operator<<(const File& src)
// {
// 	file = src;
// 	file.Open();
// 	setFlags(FILE_SENDING);
// 	return (*this);
// }

BufferQueController	&BufferQueController::operator<<(File& src)
{
	this->incoming << src;
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