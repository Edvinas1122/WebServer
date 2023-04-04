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
		FileSend(Connection *connection, ServiceProcess *followingProcess, std::string const &path):
					ServiceProcess(connection, followingProcess), fileToSend(path.c_str()) {};
		virtual ~FileSend() {};

	bool	Handle();
};

class	FileReceive : public ServiceProcess
{
	private:
		File	fileToReceive;
		size_t	lenght;
	public:
		FileReceive(Connection *connection, std::string const &path, size_t const &len):
						ServiceProcess(connection), fileToReceive(path.c_str()), lenght(len) {
			fileToReceive.Create();
		};
		virtual ~FileReceive() {};

	bool	Handle();
};

#endif