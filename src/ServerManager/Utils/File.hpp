#include <includes.hpp>

#ifndef FILE_HPP
# define FILE_HPP

class	File: std::fstream
{
	public:
		File(const char *path): std::fstream(path, std::ios::in) {};
		File() {};
		virtual ~File() {};

		void	Create(const char *path);
		bool	Open(const char *path);

		std::string	GetContents();
		std::string	GetContentsBuffer(const int bytes);
		std::string	GetContentsBuffer();
};

class	FileProcessor: public File 
{
	public:
		std::string Get(std::string (*processor)(std::string));
};

#endif