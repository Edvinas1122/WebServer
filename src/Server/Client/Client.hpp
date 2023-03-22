#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <includes.hpp>
# include <TCP.hpp>
# include <File.hpp>

# define FILE_TRANSFERED 0
# define FILE_DOWNLOADING 1
# define FILE_SENDING 2

class Tracer
{
	private:
		int	flags;
	public:
		Tracer(const Tracer &src) {
			flags = src.flags;
		};
		Tracer() {
			flags = FILE_TRANSFERED;
		};
	protected:
		void setFlags(const int flag) {
			flags = flag;
		};

	public:
		bool	checkFlag(const int flag) const {
			return (flags == flag);
		};
};

class	BufferQueController: public Tcp, public Tracer
{
	private:
		File	file;
		bool	incoming_transmission;

	public:
		BufferQueController() {};
		BufferQueController(const int fd): Tcp(fd), Tracer() {};
		BufferQueController(const BufferQueController &src): Tcp(src.fd), Tracer(src) {
		};
		~BufferQueController() {};

		bool	uploadBufferNotEmpty() const;
		bool	sendPacket();
		bool	receivePacket();

		void	Download(std::string const &path = "") {

			incoming_transmission = true;
			setFlags(FILE_DOWNLOADING);
			file.Create(path.c_str());
		};

		friend void	operator>>(const BufferQueController& client, File& file);

	public:
		BufferQueController	&operator<<(const File& src);

		template<typename TYPE>
		BufferQueController	&operator<<(TYPE const &src) {
			std::stringstream buffer;
			buffer << src;
			outgoing.append(buffer.str(), buffer.str().length());
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

		bool	sendPacket()
		{
			try {
				return (BufferQueController::sendPacket());
			} catch (...) {
				std::cerr << "packet failed - client Close" << std::endl;
				updateTime(true);
				return (false);
			}
		};

		bool	receivePacket()
		{
			try {
				return (BufferQueController::receivePacket());
			} catch (...) {
				std::cerr << "packet recv failed - client Close" << std::endl;
				updateTime(true);
				return (false);
			}
		};

		void	setInactiveTimeOutCounter(const int counter) {
			if (counter == 0) {
				updateTime(true);
				return ;
			}
			inactiveTimeOutDurration = counter;
			updateTime();
		};

		void	updateTime(const bool timeout = false);
		time_t	getElapsedTime() const;

		/*protected*/
		int		getTimeOutDurration() {
			return (inactiveTimeOutDurration);
		};
};

#endif
