#include "Processes.hpp"

bool	FileSend::Handle()
{
	theConnection() << fileToSend;
	if (fileToSend.GetRemainingLen())
		return (true);
	std::cout << "send completed" << std::endl;
	return (false);
}

bool	FileReceive::Handle()
{
	theConnection() >> fileToReceive;
	if (lenght <= fileToReceive.GetSize())
	{
		std::cout << "file received" << std::endl;
		return (false);
	}
	return (true);
}