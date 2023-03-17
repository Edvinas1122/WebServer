#include <Client.hpp>

// Client::Client(const int clientFd, const struct sockaddr_in &socketAddress): Tcp(clientFd), socketAddress(socketAddress)
// {
// 	updateTime();
// 	std::cout << "Client created " << clientFd << std::endl;
// }

Client::Client(const int clientFd, const struct sockaddr_in &socketAddress, const std::string &port): BufferQueController(clientFd), socketAddress(socketAddress), port(port)
{
	lst_msg_time.tv_sec = 10; // prevent timeouted non update error
	updateTime();
	std::cout << "Client created " << clientFd << std::endl;
}

Client::~Client()
{

}

void	Client::updateTime(const bool timeout)
{
	// if (!timeout)
	if (!timeout && !(lst_msg_time.tv_sec == 0 && lst_msg_time.tv_usec == 0))
		gettimeofday(&lst_msg_time, NULL);
	else
		memset(&lst_msg_time, 0, sizeof(lst_msg_time));
}

time_t	Client::getElapsedTime() const
{
	struct timeval	currentTime;

	gettimeofday(&currentTime, NULL);
	return (currentTime.tv_sec - lst_msg_time.tv_sec);
}
