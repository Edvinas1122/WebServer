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
	return (false);
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
	if (buffer.empty())
		return (true);
	if (!allowInsert(buffer.length()))
	{
		SetFollowingProcess(ErrorRespone(413));
		throw ExceededMaximumLen();
	}
	if (!chunkTrimmed)
	{
		if (CheckChunkHeader()) {
			chunkTrimmed = ChunkBeginTrimHandle();
			if (chunkTrimmed && CheckChunkEnd()) { // single buffer len file solution
				ChunkEndHandle();
				return (!chunkTrimmed);
			}
		}
		return (chunkTrimmed); // cancelation / ending or proceeding
	}
	if (CheckChunkEnd()) {
		ChunkEndHandle();
		return (!chunkTrimmed); // continuation or ending
	}
	return (true);
}

void	HTTPBufferReceive::Continue()
{
	chunkTrimmed = false;
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

bool	HTTPFileReceive::Handle()
{
	try {
		if (!HTTPBufferReceive::Handle()) // end
		{
			buffer >> file;
			return (false);
		}
	} catch (ExceededMaximumLen &e) {
		return (false);
	}
	buffer >> file;
	return (true);
}

bool	HTTPFileReceive::CheckChunkHeader()
{
	return (true);
};

bool	HTTPFileReceive::ChunkBeginTrimHandle()
{
	return (true);
}

bool	HTTPFileReceive::CheckChunkEnd()
{
	return (buffer.length() >= remainingReceiveLen);
};

void	HTTPFileReceive::ChunkEndHandle()
{
	buffer = buffer.substr(0, remainingReceiveLen);
}

/*
	HTTPDelimiterChunkedFileReceive
	this receive type does not specify name in an Http header
	it specifies approx len which can be usefull to lessen comparison
	opperations
*/
bool	HTTPDelimiterChunkedFileReceive::Handle()
{
	try {
		if (!HTTPBufferReceive::Handle()) // end
		{
			buffer >> file;
			return (false);
		}
	} catch (ExceededMaximumLen &e) {
		return (false);
	}
	if (chunkTrimmed && (approxLen > beginToMatch || buffer.find(delimiter) == std::numeric_limits<size_t>::max()))
	{
		approxLen -= buffer.length();
		buffer >> file;
		buffer.clear();
	}
	return (true);
}

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

#include "mod/contentUtils.hpp"

bool	HTTPDelimiterChunkedFileReceive::ChunkBeginTrimHandle()
{
	file.Create(updateDirIfFileExists(directory.append(getFileName(&buffer).c_str())).c_str());
	trimUntilFileBegin(buffer);
	return (true);
}

bool	HTTPDelimiterChunkedFileReceive::CheckChunkEnd()
{
	if (approxLen > beginToMatch)
		return (false);
	return (matchingDelimiter(buffer, delimiter));
}

void	HTTPDelimiterChunkedFileReceive::ChunkEndHandle()
{
	buffer = buffer.substr(0, buffer.find(delimiter) - 4);
}


/*
	Chunk type has specified filename at header, but only specifies len
	of a chunk in message
*/

bool	HTTPLenChunkedFileReceive::Handle()
{
	try {
		if (!HTTPBufferReceive::Handle()) // end
			return (false);
	} catch (ExceededMaximumLen &e) {
		return (false);
	}
	if (chunkTrimmed)
	{
		length -= buffer.length();
		buffer >> file;
	}
	return (true);
}

static size_t	HexStrToLong(std::string const &hex_str)
{
	std::istringstream iss(hex_str);
	size_t size;

	iss >> std::hex >> size;
	return (size);
}


bool	HTTPLenChunkedFileReceive::CheckChunkHeader()
{
	return ((buffer.find("\r\n") != std::string::npos));
}

bool	HTTPLenChunkedFileReceive::ChunkBeginTrimHandle()
{
	length = HexStrToLong(buffer.substr(0, buffer.find("\r\n")));
	totalLen += length;
	if (length == 0)
		return (false);
	buffer = buffer.substr(buffer.find("\r\n") + 2);
	return (true);
}

bool	HTTPLenChunkedFileReceive::CheckChunkEnd()
{
	return ((length <= buffer.length()));
}

void	HTTPLenChunkedFileReceive::ChunkEndHandle()
{
	Buffer	toFile;

	toFile = buffer.substr(0, length);
	toFile >> file;
	if (buffer.length() >= length + 2)
	{
		buffer = buffer.substr(length + 2);
		if (!buffer.empty() && buffer.substr(0, 1).find("0") != std::string::npos)
			return;
	}
	Continue();
}

#include "mod/contentUtils.hpp"

std::string	HTTPLenChunkedFileReceive::getTotalLen()
{
	return (to_string(totalLen));
}
