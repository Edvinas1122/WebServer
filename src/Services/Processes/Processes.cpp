#include "Processes.hpp"

bool	MasterProcess::Handle()
{
	ServiceProcess	*followingProcess;

	if (!theConnection().getMessage().empty())
	{
		try {
			followingProcess = RequestParse(theConnection().getMessage());
		} catch (...)
		{
			std::cerr << "Parcing failure" << std::endl;
			throw (std::exception());
		}
		if (followingProcess != NULL)
		{
			QueFollowingProcess(followingProcess);
			return (false);
		}
		throw (std::exception()); // undefined syntax
	}
	return (true);
};

bool	FileSend::Handle()
{
	theConnection() << fileToSend;
	if (fileToSend.GetRemainingLen())
		return (true);
	std::cout << "send completed" << std::endl;
	return (false);
}

/*
	Length Based transmision end determinant

	if upcoming buffer + current file size longer that 
	expected length - trim upcoming buffer
*/
bool	FileReceive::Handle()
{
	Buffer	tst;

	theConnection() >> tst;
	if (!tst.empty())
	{
		if (lenght <= fileToReceive.GetSize() + tst.length())
		{
			std::cout << fileToReceive.GetSize() << std::endl;
			std::cout << tst.length() << std::endl;
			tst = tst.substr(0, tst.length() - (fileToReceive.GetSize() + tst.length() - lenght));
			tst >> fileToReceive;
			std::cout << "file received: size " << fileToReceive.GetSize() << std::endl;
			return (false);
		}
		tst >> fileToReceive;
		std::cout << "Packet received: size: " << fileToReceive.GetSize() << " expected size: " << lenght << std::endl;

	}
	if (lenght <= fileToReceive.GetSize())
	{
		std::cout << "file received: size " << fileToReceive.GetSize() << std::endl;
		return (false);
	}
	return (true);
}
