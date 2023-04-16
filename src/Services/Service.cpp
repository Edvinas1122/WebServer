#include "Service.hpp"

// ServiceProcess::~ServiceProcess()
// {
// 	ServiceProcess	*process = GetLastProcess();

// 	while (process)
// 	{
// 		delete process;
// 		process = GetLastProcess();
// 	}
// }
ServiceProcess::~ServiceProcess()
{
	if (followingProcess)
		delete followingProcess;
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

void	ServiceProcess::PushBackFollowingProcess(ServiceProcess *followingProcess)
{
	GetLastProcess()->SetFollowingProcess(followingProcess);
}

void	ServiceProcess::setTimeOutDurration(const int timeOutDurration)
{
	if (followingProcess)
		followingProcess->setTimeOutDurration(timeOutDurration);
	TimeOut::setTimeOutDurration(timeOutDurration);
}

ServiceProcess	*ServiceProcess::GetLastProcess()
{
	ServiceProcess	*process = this->followingProcess;

	if (!process)
		return (NULL);
	while (process->followingProcess)
		process = process->followingProcess;
	return (process);
}

void	ServiceProcess::ScheduleFollowUp(void (*promise)(ServiceProcess *, ServiceProcess *))
{
	future = promise;
}

void	ServiceProcess::handleFollowUp()
{
	if (future)
		future(this, followingProcess);
	future = NULL;
}