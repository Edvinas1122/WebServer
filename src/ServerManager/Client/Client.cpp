#include <ServerClasses.hpp>
#include <AttributeGetter.hpp>

Client::Client(const int clientFd, const struct sockaddr_in &socketAddress): Tcp(clientFd)
{
	this->socketAddress = socketAddress;
	updateTime();
}

Client::~Client() {}

void	Client::updateTime()
{
	gettimeofday(&lst_msg_time, NULL);
}

time_t	Client::getElapsedTime() const
{
	struct timeval	currentTime;

	gettimeofday(&currentTime, NULL);
	return (currentTime.tv_sec - lst_msg_time.tv_sec);
}

/*
	Void
*/
void	Client::ProcessMessage()
{
	MessageProcessor::getParsed<HttpRequest>(Tcp::getMessage());
	*this << "DOG BARKS!";
}