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

/*
	Length Based transmision end determinant
*/
bool	HTTPFileReceive::Handle()
{
	if (!beginTrimmed)
		return (BeginTrimHandle());
	else
		return (FileReceive::Handle());
	return (true);
}

#include "mod/contentUtils.hpp"

bool	HTTPFileReceive::BeginTrimHandle()
{
	std::string	header_tmp;

	buffer >> header_tmp;
	if (header_tmp.find(delimiter, header_tmp.find(delimiter) + 1) != std::string::npos)
	{
		lenght -= unchunkBegining(buffer, delimiter).length() + 4;
		std::cout << "Receive length: " << lenght << std::endl;
		buffer = unchunkBegining(buffer, delimiter);
		trimUntilFileBegin(buffer);
		buffer >> fileToReceive;
		beginTrimmed = true;
		return (FileReceive::Handle());
	}
	else {
		Buffer	tmp;

		theConnection() >> tmp;
		buffer << tmp;
		return (true);
	}
}