#include "Processes.hpp"

bool	BufferRequest::Handle()
{
	ServiceProcess	*followingProcess;

	if (!theConnection().getMessage().empty())
	{
		requestBuffer.append(theConnection().getMessage());
		if (!RequestCompleted(requestBuffer))
			return (true);
		try {
			followingProcess = RequestParse(requestBuffer);
		} catch (...)
		{
			std::cerr << "Parcing failure" << std::endl;
			throw (std::exception());
		}
		if (followingProcess != NULL)
		{
			requestBuffer.clear();
			SetFollowingProcess(followingProcess);
			return (false);
		}
		// throw (std::exception()); // undefined syntax
	}
	return (true);
};

bool	FileSend::Handle()
{
	theConnection() << fileToSend;
	if (fileToSend.GetRemainingLen())
		return (true);
	return (false);
}

void	FileSend::removeFile()
{
	fileToSend.close();
	std::remove(filename.c_str());
}

/*
	Length Based transmision end determinant

	if upcoming buffer + current file size longer that 
	expected length - trim upcoming buffer
*/
bool	FileReceive::Handle()
{
	Buffer	tst;

	if (!theConnection().downloadBufferReady())
		return (true);
	theConnection() >> tst;
	if (!tst.empty())
	{
		if (lenght <= fileToReceive.GetSize() + tst.length())
		{
			tst = tst.substr(0, tst.length() - (fileToReceive.GetSize() + tst.length() - lenght));
			tst >> fileToReceive;
			return (false);
		}
		tst >> fileToReceive;
	}
	if (lenght <= fileToReceive.GetSize())
		return (false);
	return (true);
}

// #define ALLOWED_TIME_TO_START 1000

// bool	PipeSend::allowedTimePassed()
// {
// 	struct timeval	currentTime;

// 	if (!timestamp.tv_sec) {
// 		gettimeofday(&timestamp, NULL);
// 		return (false);
// 	}
// 	gettimeofday(&currentTime, NULL);
// 	if (ALLOWED_TIME_TO_START < (currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000) -
// 		(timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000))
// 		return (true);
// 	return (false);
// }

bool	PipeSend::Handle()
{
	if (!wait) { //implament timer to wait before sending determined by size
		bufferConnection();
		wait = true;
		return (true);
	}
	else if (wait)
	{
		if (!theConnection().uploadBufferReady())
			return (false); // client received data
		return (true); // buffer still uploading
	}
	return (true); // keep waiting
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
