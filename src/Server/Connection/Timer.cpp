#include "Service.hpp"

Timer::Timer()
{
	gettimeofday(&lst_msg_time, NULL);
}

void	Timer::updateTime(const bool timedout)
{
	if (!timedout && !(lst_msg_time.tv_sec == 0 && lst_msg_time.tv_usec == 0))
		gettimeofday(&lst_msg_time, NULL);
	else
		memset(&lst_msg_time, 0, sizeof(lst_msg_time));
}

time_t	Timer::getElapsedTime() const
{
	struct timeval	currentTime;

	gettimeofday(&currentTime, NULL);
	return (currentTime.tv_sec - lst_msg_time.tv_sec);
}