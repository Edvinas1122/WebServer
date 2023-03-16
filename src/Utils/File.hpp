#ifndef FILE_HPP
# define FILE_HPP
#include <includes.hpp>

class	File: public std::fstream
{
	private:
		std::string	path;
	public:
		File(): std::fstream() {};
		File(const char *path): std::fstream(path, std::ios::in), path(path) {};
		// File(const File& other): std::fstream(other.path, std::ios::in), path(other.path) {};
		File& operator=(const File& other)
		{
			path = other.path;
			// Open();
			return *this;
		};
		virtual ~File() {};

		void	Create(const char *path);
		bool	Open(const char *path);
		bool	Open() {return (Open(this->path.c_str()));};

		size_t	GetSize() {
				seekg(0, std::ios::end);
   				std::streampos fileSize = tellg();
				seekg(0, std::ios::beg);
				return (fileSize);
		};
		
		void	insertBuffer(const char* buffer, size_t len) {
			if (std::fstream::is_open())
				std::fstream::write(buffer, len);
		};

		/* Extracts */
		std::string	GetContents();
		std::string	GetContentsBuffer(const int bytes);
		std::string	GetContentsBuffer();

		/* Writes */
};

class	FileProcessor: public File 
{
	public:
		FileProcessor() {};
		FileProcessor(const char *path): File(path) {};
		virtual ~FileProcessor() {};

		std::string GetContents(std::string (*processor)(std::string const&) = default_processor);

	private:
		static std::string	default_processor(std::string const &content);
};

#endif