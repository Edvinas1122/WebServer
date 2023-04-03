#ifndef WEBSITE_HPP
# define WEBSITE_HPP

# include <includes.hpp>
# include <Service.hpp>

class	Download: public ServiceProcess
{
	private:
		File file;

	public:
		Download(Client *client, std::string const &fileDir): ServiceProcess(client), file(fileDir.c_str()) {};
		virtual ~Download() {};
	
	bool	Ready();
	void	Handle();

	private:
		virtual void	ParseRequest() {};
		virtual void	End() {};
};

class	MessageSend: public ServiceProcess
{
	private:
		const std::string	Message;

	public:
		MessageSend(Client *client, std::string const &message): ServiceProcess(client), Message(message) {};
		virtual ~MessageSend() {};

	bool	Ready() {return true;};
	void	Handle() {};
};

class	FileUpload: public ServiceProcess
{
	private:
		File	file;
		Buffer	buffer;

	public:
		FileUpload(Client *client): ServiceProcess(client) {};
		FileUpload(Client *client, std::string const &filename): ServiceProcess(client), file(filename.c_str()) {};
		~FileUpload() {};

	bool	Ready() {return true;};
	void	Handle();
};

#include <HTTP.hpp>
#include <VirtualServer.hpp>

class	WebSite: public Service
{
	public:
		WebSite() {};
		~WebSite() {};

	virtual bool	Ready(Client &client);
	virtual	void	SetResponse(Client &client);
	virtual	void	Handle(ServiceProcess *process);

	private:
		void	addServiceProcess(Client &client, HttpRequest const &request, const VirtualServer &vs);

	private:
		VirtualServers *virtualServers;
	public:
		void	SetVirtualServerMap(VirtualServers *vs);

};

#endif