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
		Buffer(): buffer() {
			buffer.reserve(1024);
		};
		Buffer(const Buffer &src): buffer(src.buffer) {};
		Buffer(const Vector &src): buffer(src) {};
		Buffer	&operator=(const Buffer &src) {
			buffer.clear();
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

		Buffer	substr(const size_t pos, const size_t len = 0) {
			if (len == 0)
				return (Vector(buffer.begin() + pos, buffer.end()));
			return (Vector(buffer.begin() + pos, buffer.begin() + pos + len));
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

		uint8_t	*operator*()
		{
			return (buffer.data());
		}

		uint8_t	operator[](int n) const
		{
			return (buffer[n]);
		}

		#define FILE_BUFFERING_SIZE 1024

		Buffer	&operator<<(File &file)
		{
			char	tmp_buffer[FILE_BUFFERING_SIZE];
			size_t	buffer_iterator;
			size_t	initial_buffer_size = buffer.size();
			size_t	remaining_len = file.GetRemainingLen();

			if (initial_buffer_size < FILE_BUFFERING_SIZE) {
				memset(tmp_buffer, 0, FILE_BUFFERING_SIZE);
				file.GetContentsAsBinaryBuffet((void *)tmp_buffer, FILE_BUFFERING_SIZE - initial_buffer_size);
				buffer_iterator = 0;
				while (buffer_iterator < FILE_BUFFERING_SIZE - initial_buffer_size && buffer_iterator < remaining_len)
				{
					buffer.push_back(tmp_buffer[buffer_iterator]); 
					buffer_iterator++;
				}
			}
			return (*this);
		};

		Buffer	&operator<<(const Buffer &src)
		{
			buffer.insert(buffer.end(), src.buffer.begin(), src.buffer.end());
			return (*this);
		};

		Buffer	&operator<<(const std::string &src)
		{
			std::vector<unsigned char>	&data = buffer;

			for (size_t i = 0; i < src.length(); ++i)
			{
				data.push_back((unsigned char)src.at(i));
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

		friend void	operator>>(Buffer &buffer, std::string& str)
		{
			const std::vector<unsigned char>	&data = buffer.buffer;

			for (size_t i = 0; i < data.size(); ++i)
			{
				str += (char)(data[i]); // while is ascii
			}
		}

		friend void	operator>>(Buffer const &buffer, std::string& str)
		{
			const std::vector<unsigned char>	&data = buffer.buffer;

			for (size_t i = 0; i < data.size(); ++i)
			{
				str += (char)(data[i]); // while is ascii
			}
		}

		friend void	operator>>(Buffer &client, File& file)
		{
			file.insertBuffer((char *)client.buffer.data(), client.buffer.size());
			client.clear();
		}

		size_t	length() {
			return (buffer.size());
		};

		size_t	find(std::string const &match, const size_t offset = 0)
		{
			size_t	iterator = offset;

			while (iterator < buffer.size())
			{
				if (testMatch(match, iterator, match.length()))
					return (iterator);
				iterator++;
			}
			return (std::numeric_limits<size_t>::max());
		};

		bool	compare(const size_t pos, const size_t len, const char *str) const
		{
			size_t	offset = pos;
			size_t	iterator = 0;

			while (iterator < len)
			{
				if (buffer[iterator + offset] != str[iterator])
					return (false);
			}
			return (true);
		}
		// size_t	find(std::string const &match, const size_t offset = 0) const
		// {
		// 	size_t	iterator = offset;

		// 	while (iterator < buffer.size() + match.length())
		// 	{
		// 		if (testMatch(match, iterator, match.length()))
		// 			return (iterator);
		// 		iterator++;
		// 	}
		// 	return (std::numeric_limits<size_t>::max());
		// };

		size_t	find_last(std::string const &match)
		{
			size_t	iterator = buffer.size() - match.length();

			while (iterator != 0)
			{
				if (testMatch(match, iterator, match.length()))
					return (iterator);
				iterator++;
			}
			return (std::numeric_limits<size_t>::max());
		};

	private:
		bool	testMatch(std::string const &match, size_t offset, size_t len)
		{
			size_t	iterator = 0;

			if (buffer.empty() || match.empty())
				return (false);
			while (iterator < len && offset + iterator < buffer.size())
			{
				if ((char)buffer[offset + iterator] != match[iterator])
					return (false);
				iterator++;
			}
			return (true);
		};
};

#endif
