#ifndef BUFFERED_RECEIVE_TYPES_HPP
# define BUFFERED_RECEIVE_TYPES_HPP

# include <includes.hpp>
# include <WebSite.hpp>

// class	HTTPFileReceive: public	HTTPBufferReceive
// {
// 	private:
// 		File				file;
// 		Buffer				buffer;
// 		const std::string	filename;
// 		size_t				len;
// 	public:
// 		HTTPFileReceive(const HTTPParser &process, std::string const &dir, const size_t exactLen):
// 						ServiceProcess(process), BufferRequest(process), filename(dir), len(exactLen) {};
// 		HTTPFileReceive(const HTTPParser &process, ServiceProcess *followingProcess, std::string const &dir, const size_t exactLen):
// 						ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess), filename(dir), len(exactLen) {};
// 		virtual	~HTTPFileReceive() {};

// 	bool	Handle();

// }

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

		size_t	approxLen;
		static const size_t	beginToMatch = 2500;
};

class	HTTPLenChunkedFileReceive : public HTTPBufferReceive
{
	private:
		File		file;
		size_t		length;
		std::string	directory;
		size_t		totalLen;

	public:
		HTTPLenChunkedFileReceive(const HTTPParser &process, const std::string &filepath):
					ServiceProcess(process), BufferRequest(process), HTTPBufferReceive(process),
					directory(filepath), totalLen(0) {file.Create(directory.c_str());};
		HTTPLenChunkedFileReceive(const HTTPParser &process, ServiceProcess *followingProcess, const std::string &filepath):
					ServiceProcess(process, followingProcess), BufferRequest(process, followingProcess), HTTPBufferReceive(process, followingProcess),
					directory(filepath), totalLen(0) {file.Create(directory.c_str());};
		virtual ~HTTPLenChunkedFileReceive() {};

	virtual bool	Handle();
	std::string		getTotalLen();

	protected:
	int		GetFileDescriptor();

	private:

	virtual bool	CheckChunkHeader();
	virtual bool	ChunkBeginTrimHandle();
	virtual bool	CheckChunkEnd();
	virtual void	ChunkEndHandle();
};

#endif