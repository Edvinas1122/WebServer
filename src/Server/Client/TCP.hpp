#ifndef TCP_HPP
# define TCP_HPP

# include <includes.hpp>
# include <Buffer.hpp>

class	Tcp
{
	protected:
		int			fd;
		sockaddr_in	socketAddress;
		Buffer		incoming;
		Buffer		outgoing;
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
	
	public:
		bool		sendPacket();
		bool		receivePacket();

	public:
		const std::string	getMessage() const {
			return (incoming);
		};

		Tcp	&operator<<(const std::string& str);
		Tcp	&operator<<(const char * str);
		Tcp	&operator<<(Buffer &buffer);
		Tcp	&operator=(Buffer &buffer);

		friend std::ostream& operator<<(std::ostream &os, Tcp &obj)
		{
			if (obj.incoming.length()) {
				os << obj.incoming;
				obj.flushIncoming();
			}
			return (os);
		};

		friend void	operator>>(Tcp& client, File& file)
		{
			client.incoming >> file;
		};

		friend void	operator>>(Tcp& client, Buffer& buffer)
		{
			buffer = client.incoming;
		};

		int	getSocket() const {
			return (fd);
		};

	private:
		void flushIncoming() {
			incoming.clear();
		};
};

#endif