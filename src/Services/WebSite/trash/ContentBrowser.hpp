#ifndef CONTENT_BROWSER_HPP
# define CONTENT_BROWSER_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <UploadService.hpp>
# include <HTTP.hpp>
#include <File.hpp>

class	Upload: public ServiceProcess
{
	private:
		File file;

	public:
		Upload(Client *client, std::string const &filename): ServiceProcess(client), file(filename) {};
		virtual ~Upload() {};
	
		void	Add() {};
		void	Handle() {};

	private:
		virtual void	ParseRequest() {};
		virtual void	End() {};

};

class	WebSite: public Service
{
	public:
		WebSite() {};
		~WebSite() {};

	virtual bool	Ready(Client &client);
	virtual	void	Serve(Client &client);
	virtual	void	Handle(Client &client);

	private:
		virtual bool		addServiceProcess(Client *client, std::string const &requested_service);
		virtual bool		clientInServiceList(Client *client, std::string const &requested_service);
		virtual process_it	*findClient(Client *client, std::string const &requested_service);
		virtual void		RemoveClient(Client *client);
};

class	ContentBrowser: public Service
{

};

class	FileUpload: public Service
{

};

#endif
