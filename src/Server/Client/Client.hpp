#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class	Tcp
{
	protected:
		int		fd;
		Buffer	incoming;
		Buffer	outgoing;
	public:
		Tcp() {};
		Tcp(const Tcp &src): fd(src.fd), incoming(src.incoming), outgoing(src.outgoing) {};
		Tcp(const int &fd): fd(fd) {};
		~Tcp() {};

	public:
		bool		uploadBufferNotEmpty() const; // outgoing empty
		bool		incomingAvailable() const 
		{
			return (!incoming.empty());
		}; // outgoing empty
		bool		sendPacket();
		bool		receivePacket();


		const std::string	getMessage() const {
			return (incoming);
		};

		Tcp	&operator<<(const std::string& str);
		Tcp	&operator<<(const char * str);

		friend std::ostream& operator<<(std::ostream &os, Tcp &obj)
		{
			if (obj.incoming.length()) {
				os << obj.incoming;
				obj.flushIncoming();
			}
			return (os);
		};

	private:
		void flushIncoming() {
			incoming.clear();
		};
};



class	BufferQueController: public Tcp
{
	private:
		File	file;
		bool	incoming_transmission;

	public:
		BufferQueController() {};
		BufferQueController(const int fd): Tcp(fd) {};
		BufferQueController(const BufferQueController &src): Tcp(src.fd) {};
		~BufferQueController() {};

		bool		uploadBufferNotEmpty() const;
		bool		sendPacket();
		bool		receivePacket();

		void	Create(const std::string &path) {
			incoming_transmission = false;
			file.Create(path.c_str());
		};

		void	Download(std::string const &path = "") {
			incoming_transmission = true;
			file.Create(path.c_str());
		};

		friend void operator>>(const BufferQueController& client, File& file);

	public:
		BufferQueController	&operator<<(const File& src);

		template<typename TYPE>
		BufferQueController	&operator<<(TYPE const &src) { // non binary in receive
			std::stringstream buffer;
			buffer << src;
			outgoing.append(buffer.str(), buffer.str().length());
			// outgoing.append(buffer.str());
			return (*this);
		};

	protected:
		void	insertBuffer(std::string const &buffer, size_t len) {
			file.insertBuffer(buffer.c_str(), len);
		};

};

class	Client: public BufferQueController
{
	private:
		sockaddr_in 		socketAddress;
		struct timeval		lst_msg_time;
		int					inactiveTimeOutDurration;
		const std::string	port;

	public:
		Client() {
			lst_msg_time.tv_sec = 10; // prevent timeouted non update error
			updateTime();
		};
		~Client();
		Client(const Client &src): BufferQueController(src), port(src.port) {
			inactiveTimeOutDurration = 100000;
			socketAddress = src.socketAddress;
			lst_msg_time.tv_sec = 10; // prevent timeouted non update error
			updateTime();
		};

		Client(const int clientFd, const struct sockaddr_in &socketAddress, const std::string &port = "default");

		void	info() const
		{
			std::cout << "Client info: \n" << "IP " << inet_ntoa(socketAddress.sin_addr) << std::endl;
		};

		void	setInactiveTimeOutCounter(const int counter) {
			inactiveTimeOutDurration = counter;
			updateTime();
		};

		void	updateTime(const bool timeout = false);
		time_t	getElapsedTime() const;

		/*protected*/
		int		getTimeOutDurration() {
			return (inactiveTimeOutDurration);
		};
		// const int					&getSocket() const;
		// const struct socketaddr_in	&getAddress() const;

};

#endif