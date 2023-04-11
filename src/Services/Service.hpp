#ifndef SERVICE_HPP
# define SERVICE_HPP

# include <includes.hpp>
# include <Connection.hpp>

class	TimeOut
{
	private:
		Timer				*timer;
		int					timeOutDurration;
		static const int	DefaultTODurration = 90;

	public:
		TimeOut(Timer *timer): timer(timer), timeOutDurration(DefaultTODurration) {};
		TimeOut(TimeOut const &src): timer(src.timer), timeOutDurration(src.timeOutDurration) {};
		~TimeOut () {};

	void	setTimeOutDurration(const int timeOutDurration);
	bool	isTimedOut() const;
	bool	ageTimedOut(const size_t &age);
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
		ServiceProcess(const ServiceProcess &src): TimeOut(src), connection(src.connection), finished(false), followingProcess(NULL) {};
		ServiceProcess(const ServiceProcess &src, ServiceProcess *followingProcess):
						TimeOut(src), connection(src.connection), finished(false), followingProcess(followingProcess) {};
		virtual ~ServiceProcess();

	bool					id(Connection *address) const;
	virtual bool			Finished() const;
	virtual bool			Handle() = 0;
	void					End();
	virtual ServiceProcess	*NextProcess();
	virtual bool			HeartBeat() {return (true);};
	// protected:
	Connection	&theConnection() {return (*connection);};
	void	setTimeOutDurration(const int timeOutDurration);
	protected:

	virtual void	SetFollowingProcess(ServiceProcess *);
	//ServicePorcess	*getFollowingProcess(); // list structure
};

class	Service
{
	public:
		Service() {};
		~Service() {};
	
	virtual ServiceProcess	*RequestParse(Connection *connection, std::string const &request) = 0;
	
	/* Name idle process types of a service */
	virtual bool			DetermineIfIdleProcessType(ServiceProcess*) {return (false);};
	virtual	size_t			TimeOutAge() const {return (20);};
};

#endif
