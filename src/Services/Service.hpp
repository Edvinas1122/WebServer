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
		Connection	*connection;
		bool		finished;
	
	public:
		ServiceProcess(Connection *connection): TimeOut(connection), connection(connection), finished(false) {};
		virtual ~ServiceProcess() {};

	bool			id(Connection *address) const;
	bool			Finished() const;
	virtual bool	Handle() = 0;
	void			End();
	// protected:
		Connection	&theConnection() {return (*connection);};
};

class	Service
{
	public:
		Service() {};
		~Service() {};
	
	virtual ServiceProcess	*RequestParse(Connection *connection, std::string const &request) = 0;

};

// class	Service
// {
// 	public:
// 		typedef	std::list<ServiceProcess*>::iterator	process_it;

// 	private:
// 		std::list<ServiceProcess*>	processList;

// 	public:
// 		Service() {};
// 		~Service();

// 	virtual process_it	FindProcess(Client &client);
// 	virtual	void		SetResponse(ServiceProcess *process) = 0;
// 	virtual	void		Handle(ServiceProcess *process) = 0;

// 	protected:
// 		void		SetProcess(ServiceProcess *service);
// 		void		RemoveClient(Client *client);
// 		process_it	findClient(Client *client);
	
// 	public:

// 	static void	ServiceDefault(Client &client, Service *service);
// };

#endif
