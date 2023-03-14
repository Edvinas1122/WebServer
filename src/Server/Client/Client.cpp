#include <Client.hpp>

Client::Client(const int clientFd, const struct sockaddr_in &socketAddress): Tcp(clientFd)
{
	this->socketAddress = socketAddress;
	updateTime();
	std::cout << "Client created " << clientFd << std::endl;
}

Client::~Client()
{

}

void	Client::updateTime(const bool timeout)
{
	if (!timeout)
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

Tcp	&Tcp::operator<<(File& file)
{
	outgoing.append(file.GetContents());
	return (*this);
}