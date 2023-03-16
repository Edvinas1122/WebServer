#ifndef CLINET_HPP
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
		bool		receivePacket();
		bool		sendPacket();

		bool		ready() const; // outgoing empty

		std::string	getMessage() const {
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
		File			file;
	public:
		DataBuffer() {};
		DataBuffer(const int fd): Tcp(fd) {};
		DataBuffer(const DataBuffer &src): Tcp(src.fd)
		{ 
			// uploadInProgress = false;
		};
		~DataBuffer() {};

		bool		ready() const;
		bool		sendPacket();


		void	Create(const std::string &path) {
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

class	TransmissionTrack
{
	private:
		bool	headerSent;
		bool	keepAlive;
	public:
		TransmissionTrack() {
			headerSent = false;
			keepAlive = false;
		};

		bool	HeaderSent() const 
		{
			return (headerSent);
		};


		void	enableKeepAlive()
		{
			keepAlive = true;
		};

	/* Used by server */
		void	UpdateHeaderInfo(const bool info = true) {
			headerSent = info;
		};

		bool	keepAliveInfo() {
			return (keepAlive);
		};
};


class	Client: public DataBuffer, public TransmissionTrack
{
	private:
		sockaddr_in 		socketAddress;
		struct timeval		lst_msg_time;
		const std::string	port;

	public:
		Client() {
			updateTime();
		};
		~Client();
		Client(const Client &src): DataBuffer(src), port(src.port) {
			socketAddress = src.socketAddress;
			updateTime();
		};

		Client(const int clientFd, const struct sockaddr_in &socketAddress, const std::string &port = "default");

		void	info() const
		{
			std::cout << "Client info: \n" << "IP " << inet_ntoa(socketAddress.sin_addr) << std::endl;
		};

		void						updateTime(const bool timeout = false);
		time_t						getElapsedTime() const;

		/* to get rid */
		const int					&getSocket() const;
		const struct socketaddr_in	&getAddress() const;

};

// class	BufferController: public Client
// {
// 	public:
// 		BufferController(BufferController const &src): Client(src) {};

// 		virtual void	Download(std::string const &path = "", std::string const &boundry = "------WebKitFormBoundary") {
// 			if (path.length()) {
// 				Create(path, boundry);
// 				setInfoDownloadUp();
// 			}
// 			else if (DownloadInfo())
// 				insertBuffer(incoming.c_str(), incoming.length());
// 					// updateTime(CLOSE_CLIENT);
// 		};
	


// 		static const char	*trimClosingBoundry(std::string string, std::string const &boundry)
// 		{
// 			if (string.find(boundry) != std::string::npos)
// 				string = string.substr(0, string.find_last_of(boundry));
// 			return (string.c_str());
// 		};
// };

#endif