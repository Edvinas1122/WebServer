#include <ContentBrowser.hpp>

void	UploadService::addDownload(Client *client, std::string const &filename, std::string const &chunkDelimeter)
{
	if (checkClientInList(client) == processList.end())
	{
		unchunkBegining(client, chunkDelimeter);
		processList.push_back(ServiceProcessTrace(client, filename, chunkDelimeter));
	}
}

void	UploadService::writeBufferToFile(Client *client)
{
	st_iterator	it = checkClientInList(client);

	if (it != processList.end())
	{
		it->pullToFile();
		it++;
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