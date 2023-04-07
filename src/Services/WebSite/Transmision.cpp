#include "WebSite.hpp"

bool	HTTPFileSend::Handle()
{
	if (!theConnection().downloadBufferReady())
		return (FileSend::Handle());
	else
	{
		HTTPParser::Handle();
		return (true);
	}
}

bool	HTTPFileReceiveReport::Handle()
{
	theConnection() << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 27\r\n\r\n";
	theConnection() << "File uploaded successfully.";
	return (true);
}

// static void	unchunkEnd(Buffer &buffer, std::string const &delimeter)
// {
// 	buffer = buffer.substr(0, buffer.find(std::string("--") + delimeter));
// 	buffer = buffer.substr(buffer.find_last("\r\n"));
// }

/*
	Length Based transmision end determinant
*/
bool	HTTPFileReceive::Handle()
{
	if (!buffer.empty())
		buffer >> fileToReceive;
	if (theConnection().downloadBufferReady())
		return (FileReceive::Handle());
	return (true);
}