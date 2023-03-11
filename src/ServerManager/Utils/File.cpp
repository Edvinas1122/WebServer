#include <File.hpp>

void	File::Create(const char *path)
{
	std::fstream::open(path, std::ios::out | std::ios::trunc);
};

bool	File::Open(const char *path)
{
	std::fstream::open(path, std::ios::in);
	return (std::fstream::is_open());
};

std::string File::GetContents()
{
	std::string data;

	if (std::fstream::is_open()) {
		std::istreambuf_iterator<char> fileIterator(*this);
		data = std::string(fileIterator, std::istreambuf_iterator<char>());
	}
	return (data);
}

std::string File::GetContentsBuffer(const int bytes)
{
	std::string	data;

	if (std::fstream::is_open()) {
		char *buffer = new char[bytes + 1];

		memset(buffer, 0, bytes + 1);
		std::fstream::read(buffer, bytes);
		data = buffer;
		delete[] buffer;
	}
	return (data);
}

std::string File::GetContentsBuffer()
{
	char		buffer[1025];
	std::string	data;

	memset(buffer, 0, 1025);
	if (std::fstream::is_open()) {
		std::fstream::read(buffer, 1024);
	}
	return (buffer);
}

std::string FileProcessor::GetContents(std::string (*processor)(std::string const&))
{
	return (processor(File::GetContents()));
}

std::string	FileProcessor::default_processor(std::string const &content)
{
	return (content);
}