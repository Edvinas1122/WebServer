#ifndef BUFFER_HPP
# define BUFFER_HPP

# include <includes.hpp>
# include <File.hpp>

class	Buffer
{
	public:
		typedef std::vector<u_int8_t>	Vector;
	private:
		Vector	buffer;
	public:
		Buffer() {
			buffer.reserve(1024);
		};
		Buffer(const Buffer &src): buffer(src.buffer) {};
		Buffer(const Vector &src): buffer(src) {};
		Buffer	&operator=(const Buffer &src) {
			buffer.assign(src.buffer.begin(), src.buffer.end());
			return (*this);
		};
		~Buffer() {};

		template <typename TYPE>
		void	append(const TYPE &src, size_t lenght)
		{
			size_t	it = 0;

			while (it < lenght)
			{
				buffer.push_back(src[it]);
				it++;
			}
		};

		Buffer	substr(size_t pos, size_t len) {
			return (Vector(buffer.begin() + pos, buffer.begin() + len));
		};

		void	*data() {
			return reinterpret_cast<void*>(buffer.data());
		};

		void	clear() {
			buffer.clear();
		};

		bool	empty() const {
			return (buffer.empty());
		}

		operator std::string() const {
			return std::string(buffer.begin(), buffer.end());
		};

		#define FILE_BUFFERING_SIZE 1024

		Buffer	&operator<<(File &file)
		{
			char	tmp_buffer[FILE_BUFFERING_SIZE];
			size_t	buffer_iterator;
			size_t	initial_buffer_size = buffer.size();

			if (initial_buffer_size < FILE_BUFFERING_SIZE) {
				memset(tmp_buffer, 0, FILE_BUFFERING_SIZE);
				file.GetContentsAsBinaryBuffet((void *)tmp_buffer, FILE_BUFFERING_SIZE - initial_buffer_size);
				buffer_iterator = 0;
				while (buffer_iterator < FILE_BUFFERING_SIZE - initial_buffer_size)
				{
					buffer.push_back(tmp_buffer[buffer_iterator]); 
					buffer_iterator++;
				}
			}
			return (*this);
		};

		friend std::ostream& operator<<(std::ostream& os, const Buffer& buffer) {
			const std::vector<unsigned char>	&data = buffer.buffer;

			for (size_t i = 0; i < data.size(); ++i)
			{
				os << (char)(data[i]);
			}
			return os;
		};

		size_t	length() {
			return (buffer.size());
		};

};

#endif