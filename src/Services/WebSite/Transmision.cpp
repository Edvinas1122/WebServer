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

static void	appendToBuffer(Connection *connection, Buffer *buffer)
{
	Buffer	tmp;

	*connection >> tmp;
	*buffer << tmp;
}

bool	HTTPBufferReceive::Handle()
{
	appendToBuffer(&theConnection(), &buffer);
	if (!chunkTrimmed)
	{
		if (CheckChunkHeader()) {
			ChunkBeginTrimHandle();
			chunkTrimmed = true;
		}
		return (true);
	}
	if (CheckChunkEnd()) {
		ChunkEndHandle();
		return (false);
	}
	return (true);
}

bool	matchingDelimiter(Buffer &speciment, std::string const &delimiter)
{
	std::string	sample;

	speciment >> sample;
	if (speciment.find(delimiter) != std::string::npos &&
		sample.compare(speciment.find(delimiter), delimiter.length(), delimiter) == 0) {
		return (true);
	}
	return (false);
}

#include "BufferedReceiveTypes.hpp"

bool	HTTPDelimiterChunkedFileReceive::CheckChunkHeader()
{
	return (matchingDelimiter(buffer, delimiter));
}

const std::string	getFileName(Buffer const *buffer)
{
	std::string	data;

	*buffer >> data;
	return (data.substr(data.find("filename=") + 10, data.substr(data.find("filename=") + 11).find('"') + 1));
}

static void	trimUntilFileBegin(Buffer &buffer)
{
	buffer = buffer.substr(buffer.find("Content-Type:"));
	buffer = buffer.substr(buffer.find("\r\n") + 4);
};

void	HTTPDelimiterChunkedFileReceive::ChunkBeginTrimHandle()
{
	file.Create(directory.append(getFileName(&buffer).c_str()).c_str());
	trimUntilFileBegin(buffer);
}

bool	HTTPDelimiterChunkedFileReceive::Handle()
{
	if (!HTTPBufferReceive::Handle())
	{
		buffer >> file;
		return (false);
	}
	if (chunkTrimmed && buffer.find(delimiter) == std::string::npos)
	{
		buffer >> file;
		buffer.clear();
	}
	return (true);
}

bool	HTTPDelimiterChunkedFileReceive::CheckChunkEnd()
{
	return (matchingDelimiter(buffer, delimiter));
}

void	HTTPDelimiterChunkedFileReceive::ChunkEndHandle()
{
	buffer = buffer.substr(0, buffer.find(delimiter) - 4);
	// buffer = buffer.substr(0, buffer.find_last("\r\n"));
}



// static size_t	HexStrToLong(std::string const &hex_str) //////////////////////////////
// {
// 	std::istringstream iss(hex_str);
// 	size_t size;

// 	iss >> std::hex >> size;
// 	return (size);
// }
