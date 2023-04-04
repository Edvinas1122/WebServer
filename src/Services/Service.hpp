#ifndef SERVICE_HPP
# define SERVICE_HPP

# include <includes.hpp>
# include <Connection.hpp>

class	TimeOut
{
	private:
		Timer				*timer;
		int					timeOutDurration;
		static const int	DefaultTODurration = 20;

	public:
		TimeOut(Timer *timer): timer(timer), timeOutDurration(DefaultTODurration) {};
		~TimeOut () {};

	void	setTimeOutDurration(const int timeOutDurration);
	bool	isTimedOut() const;
};

class	ServiceProcess: public TimeOut
{
	private:
		Connection		*connection;
		bool			finished;
		ServiceProcess	*followingProcess;

	public:
		ServiceProcess(Connection *connection): 
						TimeOut(connection), connection(connection), finished(false), followingProcess(NULL) {};
		ServiceProcess(Connection *connection, ServiceProcess *followingProcess): 
						TimeOut(connection), connection(connection), finished(false), followingProcess(followingProcess) {};
		virtual ~ServiceProcess();

	bool					id(Connection *address) const;
	bool					Finished() const;
	virtual bool			Handle() = 0;
	void					End();
	virtual ServiceProcess	*NextProcess();
	// protected:
	Connection	&theConnection() {return (*connection);};

	protected:

	virtual void	QueFollowingProcess(ServiceProcess *);
};

class	Service
{
	public:
		Service() {};
		~Service() {};
	
	virtual ServiceProcess	*RequestParse(Connection *connection, std::string const &request) = 0;
};

#endif
