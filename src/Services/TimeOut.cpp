#include "Service.hpp"

void	TimeOut::setTimeOutDurration(const int timeOutDurration)
{
	if (timeOutDurration == 0) {
		timer->updateTime(true);
		return ;
	}
	this->timeOutDurration = timeOutDurration;
	// timer->updateTime();
};

bool	TimeOut::isTimedOut() const
{
	return (timer->getElapsedTime() > this->timeOutDurration);
};