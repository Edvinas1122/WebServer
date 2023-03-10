#include <includes.hpp>

#define RECEIVE_BUFFER_SIZE	1024

bool	receiveBufferRead(const int client_fd, std::string & message)
{
	char	buffer[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;

	memset((void*)buffer, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	bytes_read = recv(client_fd, buffer, RECEIVE_BUFFER_SIZE, 0);
	if (bytes_read > 0) {
		message.append(buffer);
		return (true);
	}
	return (false);
}