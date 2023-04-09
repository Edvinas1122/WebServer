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

bool	ExecuteFile::Handle()
{
	if (followingProcess)
		QueFollowingProcess(new	PipeSend(&theConnection(), followingProcess, executor.executeToOutPut()));
	else
		QueFollowingProcess(new	PipeSend(&theConnection(), executor.executeToOutPut()));
	followingProcess = NULL;
	return (false);
}

ExecuteFile::~ExecuteFile()
{
	if (followingProcess)
		delete (followingProcess);
}

bool	PipeSend::Handle()
{
	if (!wait) {
		bufferConnection();
		wait = true;
		return (true);
	}
	else
	{
		if (!theConnection().uploadBufferReady())
			return (false); // client received data
		return (true); // buffer still uploading
	}
}

#define PIPE_READ_BUFF_SIZE	1024

void	PipeSend::bufferConnection()
{
	char	buf[PIPE_READ_BUFF_SIZE];
	int		bytes_read;

	while (42)
	{
		memset(buf, 0, PIPE_READ_BUFF_SIZE);
		bytes_read = read(fd, buf, PIPE_READ_BUFF_SIZE);
		if (bytes_read == -1)
			throw std::exception();
		if (bytes_read > 0)
			theConnection().sendBytes(buf, bytes_read);
		if (bytes_read != PIPE_READ_BUFF_SIZE)
			break;
	}
}