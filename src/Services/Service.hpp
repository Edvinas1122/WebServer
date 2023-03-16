#ifndef SERVICE_HPP
# define SERVICE_HPP

# include <includes.hpp>


/*
	All services in the world serve requests
*/
class	Service
{
	public:
		Service() {};
		~Service() {};

	virtual	const std::string	Serve(std::string const &message) = 0;
	virtual bool				Ready() = 0;
	virtual	void				Handle() = 0;

};

#include <HTTP.hpp>

class	ContentBrowser: public Service
{
	public:
		ContentBrowser();
		~ContentBrowser();
	
	virtual	const std::string	Serve(HttpRequest const &message) {
		(void) message;
		return ("");
	};
	virtual bool				Ready() = 0;
	virtual	void				Handle() = 0;
};

#endif