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

	virtual bool				Ready(Client &client) = 0;
	virtual	void				Serve(Client &client) = 0;
	virtual	void				Handle(Client &client) = 0;

};

#include <HTTP.hpp>

class	ContentBrowser: public Service
{
	public:
		ContentBrowser() {};
		~ContentBrowser() {};
	
	virtual bool	Ready(Client &client);
	virtual	void	Serve(Client &client);
	virtual	void	Handle(Client &client);
	
	private:
		void	parseRequest(HttpRequest const &message);
};

#endif