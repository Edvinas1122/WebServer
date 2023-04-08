#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <includes.hpp>
# include <Buffer.hpp>

class	Timer
{
	private:
		struct timeval		lst_msg_time;
	public:
		Timer();
		~Timer() {};
	
	void	updateTime(const bool timedout = false);
	time_t	getElapsedTime() const;
	size_t	getElapsedTimeMS() const;
};

class	Connection: public Timer
{
	protected:
		int			fd;
		Buffer		incoming;
		Buffer		outgoing;
	public:
		Connection() {};
		Connection(const int &fd): fd(fd) {};
		Connection(const Connection &src): fd(src.fd), incoming(src.incoming), outgoing(src.outgoing) {};
		Connection	&operator=(const Connection &src)
		{
			fd = src.fd;
			incoming = src.incoming;
			outgoing = src.outgoing;
			return (*this);
		};
		~Connection() {};

	virtual bool	sendPacket() = 0;
	virtual bool	receivePacket() = 0;

	const std::string	getMessage() const;
	bool				uploadBufferReady() const;
	bool				downloadBufferReady() const;

	Connection	&operator<<(const std::string& str);
	Connection	&operator<<(const char * str);
	Connection	&operator<<(const size_t &number);
	Connection	&operator<<(Buffer &buffer);
	Connection	&operator=(Buffer &buffer);
	Connection	&operator<<(File &src);

	friend std::ostream&	operator<<(std::ostream &os, Connection &obj);
	friend void				operator>>(Connection& connection, Buffer& buffer) {
		buffer = connection.incoming;
	};
	friend void				operator>>(Connection& connection, std::string& str) {
		connection.incoming >> str;
	};
	friend void				operator>>(Connection& connection, File& file) {
		connection.incoming >> file;
	};

	// protected:
		void flushIncoming() {
			incoming.clear();
		};
	bool	operator==(const Connection& other) const {
		return (fd == other.fd);
	}
};

#endif