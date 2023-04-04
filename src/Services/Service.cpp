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

void	ServiceProcess::QueFollowingProcess(ServiceProcess *followingProcess)
{
	if (this->followingProcess)
		delete (this->followingProcess);
	this->followingProcess = followingProcess;
}