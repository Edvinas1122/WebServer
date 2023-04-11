#include "Service.hpp"

ServiceProcess::~ServiceProcess()
{
	if (followingProcess)
		delete (followingProcess);
}

ServiceProcess	*ServiceProcess::NextProcess()
{
	ServiceProcess	*tmp = followingProcess;

	followingProcess = NULL;
	return (tmp);
};

void	ServiceProcess::SetFollowingProcess(ServiceProcess *followingProcess)
{
	if (this->followingProcess)
		delete (this->followingProcess);
	this->followingProcess = followingProcess;
}

void	ServiceProcess::setTimeOutDurration(const int timeOutDurration)
{
	if (followingProcess)
		followingProcess->setTimeOutDurration(timeOutDurration);
	TimeOut::setTimeOutDurration(timeOutDurration);
}