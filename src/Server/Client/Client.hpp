#ifndef CLINET_HPP
# define CLIENT_HPP

# include <includes.hpp>
# include <File.hpp>

class	Tcp
{
	protected:
		int			fd;
		std::string	incoming;
		std::string	outgoing;
	public:
		Tcp() {};
		Tcp(const int &fd): fd(fd) {};

	public:
		bool		receivePacket();
		bool		sendPacket();

		bool		ready() const; // outgoing empty

		std::string	getMessage() const {
			return (incoming);
		};

		Tcp	&operator<<(const std::string& str);
		Tcp	&operator<<(File& file);

		friend std::ostream& operator<<(std::ostream &os, Tcp &obj)
		{
			if (obj.incoming.length()) {
				os << obj.incoming;
				obj.flushIncoming();
			}
			return os;
		}
	private:
		void flushIncoming() {
			incoming.clear();
		}
};

class	ProcessTrack
{
	private:
		bool	headerSent;
		bool	keepAlive;
	public:
		ProcessTrack() {
			headerSent = false;
			keepAlive = false;
		};

		bool	HeaderSent() const 
		{
			return (headerSent);
		};

		bool	keepAliveInfo() {
			return (keepAlive);
		};
		
		void	UpdateHeaderInfo(const bool info = true) {
			headerSent = info;
		};

};


class	Client: public Tcp, public ProcessTrack {
	// public:
		// Response		response;
	private:
		// HttpRequest		request;
		sockaddr_in 		socketAddress;
		struct timeval		lst_msg_time;
		const std::string	port;

	public:
		Client() {};
		~Client();
		Client(const int clientFd, const struct sockaddr_in &socketAddress);
		Client(const int clientFd, const struct sockaddr_in &socketAddress, const std::string &port);

		void	info() const
		{
			std::cout << "Client info: \n" << "IP " << inet_ntoa(socketAddress.sin_addr) << std::endl;
		};

		void						updateTime(const bool timeout = false);
		time_t						getElapsedTime() const;
		/**/
		const int					&getSocket() const;
		const struct socketaddr_in	&getAddress() const;
		// const HttpRequest			&getRequest() const; // debug purposes

		// bool						ProcessMessage();
		void						setSocket(int const &socketFd);
		void						setAddress(sockaddr_in const &socketAddress);
		// void						setServer(VirtualServer *virtualServer);
		/*
			build response from a request
		*/

		// Client	&operator<<(const std::string& str) {
		// 		outgoing.append(str);
		// 		return (*this);
		// };

};

#endif