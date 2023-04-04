#include "Processes.hpp"

bool	FileSend::Handle()
{
	theConnection() << fileToSend;
	if (fileToSend.GetRemainingLen())
		return (true);
	return (false);
}