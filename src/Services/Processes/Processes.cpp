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

bool	PipeSend::Handle()
{
	if (!wait) { //implament timer to wait before sending determined by size
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

// bool	BufferReceive::Handle()
// {
	
// }