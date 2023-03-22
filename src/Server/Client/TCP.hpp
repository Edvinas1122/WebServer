#ifndef TCP_HPP
# define TCP_HPP

# include <includes.hpp>
# include <Buffer.hpp>

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
	
	protected:
		bool		sendPacket();
		bool		receivePacket();

	public:
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

		int	getSocket() const {
			return (fd);
		};

	private:
		void flushIncoming() {
			incoming.clear();
		};
};

#endif