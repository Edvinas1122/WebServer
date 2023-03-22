#ifndef CONTENT_BROWSER_HPP
# define CONTENT_BROWSER_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Client.hpp>
# include <HTTP.hpp>

class	ServiceProcessTrace
{
	private:
		int			id;
		std::string	request;
	public:
		bool	isClient(const Client &client);
};

class	ServiceTracer
{
	public:
		typedef	std::vector<ServiceProcessTrace>::iterator	st_iterator;

	private:
		std::vector<ServiceProcessTrace>	processList;

	protected:
		void	traceClient(Client &client);

	private:
		void	addToList();
		st_iterator	findClient(const Client &client);
};

class	ContentBrowser: public Service, public ServiceTracer
{
	public:
		ContentBrowser() {};
		~ContentBrowser() {};

	virtual bool	Ready(Client &client);
	virtual	void	Serve(Client &client);
	virtual	void	Handle(Client &client);
	
	private:
		void	gate(Client &client);
	private:
		void	parseRequest(HttpRequest const &message);
};

#endif