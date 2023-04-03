#include "Service.hpp"

bool	ServiceProcess::id(Connection *address) const {
	return ((connection == address));
};

bool	ServiceProcess::Finished() const
{
	return (finished);
};

void	ServiceProcess::End()
{
	finished = true;
};