#include <File.hpp>

void	File::Create(const char *path)
{
	if (std::fstream::is_open())
	{
		std::fstream::close();
		std::fstream::clear();
	}
	this->path = path;
	std::fstream::open(path, std::ios::out | std::ios::binary | std::ios::trunc);
};

bool	File::Open(const char *path)
{
	if (std::fstream::is_open())
	{
		std::fstream::close();
		std::fstream::clear();
	}
	this->path = path;
	std::fstream::open(path, std::ios::in);
	return (std::fstream::is_open());
};

std::string File::GetContentsAsString()
{
	std::string data;

	if (!std::fstream::is_open())
		Open();
	if (std::fstream::is_open()) {
		std::istreambuf_iterator<char> fileIterator(*this);
		data = std::string(fileIterator, std::istreambuf_iterator<char>());
	}
	return (data);
}

void	File::GetContentsAsBinaryBuffet(void *input, size_t len)
{
	memset(input, 0, len);
	if (std::fstream::is_open()) {
		std::fstream::read((char *)input, len);
	}
	remaining -= gcount();
}

std::string FileProcessor::GetContents(std::string (*processor)(std::string const&))
{
	return (processor(File::GetContentsAsString()));
}

std::string	FileProcessor::default_processor(std::string const &content)
{
	return (content);
}