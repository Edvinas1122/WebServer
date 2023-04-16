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
						TimeOut(connection), connection(connection), finished(false), followingProcess(NULL) {future = NULL;};
		ServiceProcess(Connection *connection, ServiceProcess *followingProcess): 
						TimeOut(connection), connection(connection), finished(false), followingProcess(followingProcess) {future = NULL;};
		ServiceProcess(const ServiceProcess &src): TimeOut(src), connection(src.connection), finished(false), followingProcess(NULL) {future = NULL;};
		ServiceProcess(const ServiceProcess &src, ServiceProcess *followingProcess):
						TimeOut(src), connection(src.connection), finished(false), followingProcess(followingProcess) {future = NULL;};
		virtual ~ServiceProcess();

	bool					id(Connection *address) const;
	virtual bool			Finished() const;
	virtual bool			Handle() = 0;
	virtual void			End();
	virtual ServiceProcess	*NextProcess();
	virtual bool			HeartBeat() {return (true);};

	// protected:
	Connection	&theConnection() {return (*connection);};
	void		setTimeOutDurration(const int timeOutDurration);

	virtual void		handleFollowUp();

	void	ScheduleFollowUp(void (*promise)(ServiceProcess *, ServiceProcess *, Connection *));
	protected:

	virtual void	SetFollowingProcess(ServiceProcess *);
	virtual void	PushBackFollowingProcess(ServiceProcess *);


	private:
	ServiceProcess	*GetLastProcess();
	void (*future)(ServiceProcess *, ServiceProcess *, Connection *);
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
