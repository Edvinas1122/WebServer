#include <ServerClasses.hpp>

#define RECEIVE_BUFFER_SIZE 1024

void	TcpMessage::receivePacket()
{
	char	mesg[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	bytes_read = recv(fd, mesg, RECEIVE_BUFFER_SIZE, 0);
	if (bytes_read > 0) {
		message.append(mesg);
	}
}