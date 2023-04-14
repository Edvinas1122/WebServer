#ifndef BUFFERED_RECEIVE_TYPES_HPP
# define BUFFERED_RECEIVE_TYPES_HPP

# include <includes.hpp>
# include <WebSite.hpp>

class	HTTPDelimiterChunkedFileReceive : public HTTPBufferReceive
{
	private:
		File		file;
		std::string	delimiter;
		std::string	directory;

	public:
		HTTPDelimiterChunkedFileReceive(const HTTPParser &process,
											std::string const &delimiter, std::string const &dir):
					ServiceProcess(process), BufferRequest(process), HTTPBufferReceive(process),
					delimiter(delimiter), directory(dir) {};

		HTTPDelimiterChunkedFileReceive(const HTTPParser &process, ServiceProcess *followingProcess,
											std::string const &delimiter, std::string const &dir):
					ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess),
					HTTPBufferReceive(process, followingProcess), delimiter(delimiter), directory(dir) {};
		virtual ~HTTPDelimiterChunkedFileReceive() {};

	bool	Handle();

	private:

	virtual bool	CheckChunkHeader();
	virtual bool	ChunkBeginTrimHandle();
	virtual bool	CheckChunkEnd();
	virtual void	ChunkEndHandle();
};

class	HTTPLenChunkedFileReceive : public HTTPBufferReceive
{
	private:
		File		file;
		size_t		length;
		std::string	directory;

	public:
		HTTPLenChunkedFileReceive(const HTTPParser &process, const std::string &filepath):
					ServiceProcess(process), BufferRequest(process), HTTPBufferReceive(process),
					directory(filepath) {file.Create(directory.c_str());};
		HTTPLenChunkedFileReceive(const HTTPParser &process, ServiceProcess *followingProcess, const std::string &filepath):
					ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess), HTTPBufferReceive(process, followingProcess),
					directory(filepath) {file.Create(directory.c_str());};
		virtual ~HTTPLenChunkedFileReceive() {};

	bool	Handle();

	private:

	virtual bool	CheckChunkHeader();
	virtual bool	ChunkBeginTrimHandle();
	virtual bool	CheckChunkEnd();
	virtual void	ChunkEndHandle();
};

#endif