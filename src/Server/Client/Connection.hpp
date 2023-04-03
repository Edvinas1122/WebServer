#ifndef TCP_HPP
# define TCP_HPP

# include <includes.hpp>
# include <Buffer.hpp>

class	Connection
{
	protected:
		int		fd;
		Buffer	incoming;
		Buffer	outgoing;
	public:
		Connection() {};
		Connection(const Connection &src): fd(src.fd), incoming(src.incoming), outgoing(src.outgoing) {};
		Connection(const int &fd): fd(fd) {};
		~Connection() {};

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

		Connection	&operator<<(const std::string& str);
		Connection	&operator<<(const char * str);
		Connection	&operator<<(Buffer &buffer);
		Connection	&operator=(Buffer &buffer);

		friend std::ostream& operator<<(std::ostream &os, Connection &obj)
		{
			if (obj.incoming.length()) {
				os << obj.incoming;
				obj.flushIncoming();
			}
			return (os);
		};

		friend void	operator>>(Connection& client, File& file)
		{
			client.incoming >> file;
		};

		friend void	operator>>(Connection& client, Buffer& buffer)
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

class TCP : public Connection
{
	public:
		TCP() {};
		~TCP() {};

	bool	sendPacket();
	bool	receivePacket();
}

#endif