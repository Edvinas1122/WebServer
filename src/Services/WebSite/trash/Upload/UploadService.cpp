#include <ContentBrowser.hpp>
#include "mod/chunkHandlers.hpp"

static std::string	getConentFileName(std::string const &text)
{
	size_t	begin = text.find("filename=") + strlen("filename=") + 1;
	size_t	end = text.length() - begin - 1;

	return (text.substr(begin, end));
}

void	UploadService::Handle(Client *client)
{
	if (fileBegin(client))
		addDownload(client, std::string("/home/WebServer/http/upload/") + getConentFileName(client->getMessage()));
	if (!fileEnd(client))
		writeBufferToFile(client);
	else {
		writeBufferToFile(client, unchunkEnd);
		closeDownload(client);
		client->setInactiveTimeOutCounter(0);
		*client << "HTTP/1.1 200 OK\r\nConnection\r\n\r\n";
		std::cout << std::endl;
		std::cout << "file received" << std::endl;
	}
};


void	UploadService::addClientProcess(Client *client, std::string const &chunkDelimeter)
{
	if (checkClientInList(client) == processList.end())
		processList.push_back(ServiceProcessTrace(client, chunkDelimeter));
}

// void	UploadService::addClientProcess(Client *client, std::string const &filename, std::string const &chunkDelimeter, void (*process)(Client *, std::string const &))
// {
// 	if (checkClientInList(client) == processList.end())
// 	{
// 		if (process != NULL)
// 			process(client, chunkDelimeter);
// 		processList.push_back(ServiceProcessTrace(client, filename, chunkDelimeter));
// 	}
// }

void	UploadService::addDownload(Client *client, std::string const &filedir)
{
	checkClientInList(client)->setFile(filedir);
	unchunkBegining(client, checkClientInList(client)->showDelimiter());
}

void	UploadService::writeBufferToFile(Client *client, void (*process)(Client *, std::string const &))
{
	st_iterator	it = checkClientInList(client);

	if (it != processList.end())
	{
		if (process != NULL)
			it->pullToFile(process);
		else
			it->pullToFile();
	}
}

UploadService::st_iterator	UploadService::checkClientInList(const Client *client)
{
	st_iterator it = processList.begin();

	while (it != processList.end())
	{
		if (it->id(client))
			return (it);
		it++;
	}
	return (it);
}

void	UploadService::closeDownload(Client *client)
{
	processList.erase(checkClientInList(client));
}

bool	UploadService::fileEnd(Client *client)
{
	Buffer buffer;

	*client >> buffer;
	if (buffer.find(checkClientInList(client)->showDelimiter()) != std::numeric_limits<size_t>::max())
		return (true);
	return (false);
};

bool	UploadService::fileBegin(Client *client)
{
	if (!checkClientInList(client)->taskInfo())
		return (true);
	return (false);
};