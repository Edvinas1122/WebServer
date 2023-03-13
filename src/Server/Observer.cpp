#include <Server.hpp>

Observer::Observer()
{
	vector.reserve(MAX_CONNECTIONS);
#ifdef TERMINAL
	insertFileDescriptor(STDIN_FILENO);
#endif
}

int	Observer::Poll(const bool timeOutOn)
{
	int	info;

	info = poll(vector.data(), vector.size(), TIMEOUT * (timeOutOn));
	return (info);
}

void	Observer::insertFileDescriptor(const int fd, const int events, const bool asynch)
{
	struct pollfd	info;

	if (asynch)
		setAsNonBlocking(fd);
	info.fd = fd;
	info.events = events;
	info.revents = 0;
	vector.push_back(info);
}


static std::vector<struct pollfd>::iterator	findFd(std::vector<struct pollfd>::iterator it,
													const std::vector<struct pollfd>::iterator end, const int fd)
{
	while (it != end)
	{
		if (it->fd == fd)
			return (it);
		it++;
	}
	return (it);
}

void	Observer::removeFileDescriptor(const int fd)
{
	std::vector<struct pollfd>::iterator	it;

	it = findFd(vector.begin(), vector.end(), fd);
	if (it == vector.end())
		throw InvalidFd();
	close(it->fd);
	vector.erase(it);
}

bool	Observer::checkFd(const int fd, const int events)
{
	std::vector<struct pollfd>::iterator	it;

	it = findFd(vector.begin(), vector.end(), fd);
	if (it == vector.end())
		throw InvalidFd();
	return (it->revents & events);
}

void	Observer::setAsNonBlocking(const int socket_fd) const
// static void set_fd_as_non_blocking(const int socket_fd)
{
	int flags;
	
	flags = fcntl(socket_fd, F_GETFL, 0);
	if (flags == -1) {
		// handle error getting socket flags
		std::cerr << "can not set socket as non-blocking" << std::endl;
		exit (0);
	}

	flags |= O_NONBLOCK;
	if (fcntl(socket_fd, F_SETFL, flags) == -1) {
		// handle error setting socket flags
		std::cerr << "can not set socket as non-blocking" << std::endl;
		exit (0);
	}
}