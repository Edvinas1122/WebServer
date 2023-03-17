#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <includes.hpp>

class	Tcp
{
	protected:
		int			fd;
		std::string	incoming;
		std::string	outgoing;
	public:
		Tcp() {};
		Tcp(const Tcp &src): fd(src.fd), incoming(src.incoming), outgoing(src.outgoing) {};
		Tcp(const int &fd): fd(fd) {};
		~Tcp() {};

	public:
		bool		ready() const; // outgoing empty
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

# include <File.hpp>

class	DataBuffer: public Tcp
{
	private:
		File	file;
		bool	incoming_transmission;

	public:
		DataBuffer() {};
		DataBuffer(const int fd): Tcp(fd) {};
		DataBuffer(const DataBuffer &src): Tcp(src.fd) {};
		~DataBuffer() {};

		bool		ready() const;
		bool		sendPacket();
		bool		receivePacket();

		void	Create(const std::string &path) {
			incoming_transmission = false;
			file.Create(path.c_str());
		};

		void	Download(std::string const &path = "")
		{
			incoming_transmission = true;
			file.Create(path.c_str());
		};

	public:
		DataBuffer	&operator<<(const File& src);

		template<typename TYPE>
		DataBuffer	&operator<<(TYPE const &src) {
			std::stringstream buffer;
			buffer << src;
			outgoing.append(buffer.str());
			return (*this);
		};

	protected:
		void	insertBuffer(std::string const &buffer, size_t len) {
			file.insertBuffer(buffer.c_str(), len);
		};

};

class	Client: public DataBuffer
{
	private:
		sockaddr_in 		socketAddress;
		struct timeval		lst_msg_time;
		int					inactiveTimeOutDurration;
		const std::string	port;

	public:
		Client() {
			updateTime();
		};
		~Client();
		Client(const Client &src): DataBuffer(src), port(src.port) {
			inactiveTimeOutDurration = 2;
			socketAddress = src.socketAddress;
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