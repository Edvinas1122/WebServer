#ifndef SERVICE_HPP
# define SERVICE_HPP

# include <includes.hpp>
# include <Client.hpp>

/*
	All services in the world serve requests
*/
class	Service
{
	public:
		Service() {};
		~Service() {};

	virtual bool	Ready(Client &client) = 0;
	virtual	void	Serve(Client &client) = 0;
	virtual	void	Handle(Client &client) = 0;
};

#endif
