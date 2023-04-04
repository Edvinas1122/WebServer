#ifndef PROCESSES_HPP
# define PROCESSES_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <File.hpp>

class	FileSend : public ServiceProcess
{
	private:
		File	fileToSend;
	public:
		FileSend(Connection *connection, std::string const &path): ServiceProcess(connection), fileToSend(path.c_str()) {};
		virtual ~FileSend() {};

	bool	Handle();
};

class	FileReceive : public ServiceProcess
{
	private:
		File	fileToReceive;
	public:
		FileReceive(Connection *connection, std::string const &path): ServiceProcess(connection), fileToReceive(path.c_str()) {};
		virtual ~FileReceive() {};

	bool	Handle();
};


#endif