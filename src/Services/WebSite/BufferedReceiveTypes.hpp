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
											std::string const &delimiter, std::string const &dir, const size_t approx_len):
					ServiceProcess(process), BufferRequest(process), HTTPBufferReceive(process),
					delimiter(delimiter), directory(dir), approxLen(approx_len) {};

		HTTPDelimiterChunkedFileReceive(const HTTPParser &process, ServiceProcess *followingProcess,
											std::string const &delimiter, std::string const &dir, const size_t approx_len):
					ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess),
					HTTPBufferReceive(process, followingProcess), delimiter(delimiter), directory(dir), approxLen(approx_len) {};
		virtual ~HTTPDelimiterChunkedFileReceive() {};

	bool	Handle();

	private:

	virtual bool	CheckChunkHeader();
	virtual bool	ChunkBeginTrimHandle();
	virtual bool	CheckChunkEnd();
	virtual void	ChunkEndHandle();

	size_t			approxLen;
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

	virtual bool	Handle();

	protected:
	int		GetFileDescriptor();

	private:

	virtual bool	CheckChunkHeader();
	virtual bool	ChunkBeginTrimHandle();
	virtual bool	CheckChunkEnd();
	virtual void	ChunkEndHandle();
};

// class	HTTPCGIChunkedFileReceive: public ExecuteFile, public HTTPLenChunkedFileReceive
// {
// 	public:
// 		HTTPCGIChunkedFileReceive(const HTTPParser &process, std::string const &executableFilePath, std::string const &scriptPath, std::string const &tmpPath):
// 					ServiceProcess(process), BufferRequest(process), ExecuteFile(process, executableFilePath, scriptPath),
// 					HTTPLenChunkedFileReceive(process, tmpPath) {};
// 		HTTPCGIChunkedFileReceive(const HTTPParser &process, ServiceProcess *followingProcess, std::string const &executableFilePath, std::string const &scriptPath, std::string const &tmpPath):
// 					ServiceProcess(process), BufferRequest(process), ExecuteFile(process, followingProcess, executableFilePath, scriptPath),
// 					HTTPLenChunkedFileReceive(process, tmpPath) {};

// 		virtual	~HTTPCGIChunkedFileReceive() {};

// 	bool	Handle();
// };

#endif