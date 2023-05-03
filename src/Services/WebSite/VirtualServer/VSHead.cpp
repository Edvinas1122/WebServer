#include "VirtualServer.hpp"
#include <File.hpp>
#include <configurationFileFormat.hpp>

void	VirtualServers::parseConfigurationFile(const char *path)
{
	DescendParser		parser;
	FileProcessor		configurationFile;

	configurationFile.Open(path);
	parser.setContent(configurationFile.GetContents(removeComents));
	try {
		parseServers(parser);
	} catch (...)
	{
		std::cerr << "Configuration file failure" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	VirtualServers::Info() const
{
	virtualServerMap::const_iterator	it = virtualServers.begin();

	while (it != virtualServers.end())
	{
		it->second.displayInfo();
		it++;
	}
}

VirtualServer	*VirtualServers::getServer(std::string const &port, std::string const &host)
{
	if (virtualServers.find(host) == virtualServers.end())
		return (virtualServers.begin()->second.validatePort(port));
	
	return (virtualServers.at(host).validatePort(port));
}

std::list<std::string>	VirtualServers::getPortList()
{
	std::list<std::string>		portList;
	virtualServerMap::iterator	it = virtualServers.begin(); 

	while (it != virtualServers.end())
	{
		std::list<std::string> vsPortList = it->second.getPorts();
		std::list<std::string>::iterator	vs_it = vsPortList.begin();
		while (vs_it != vsPortList.end())
		{
			if (std::find(portList.begin(), portList.end(), *vs_it) == portList.end())
				portList.push_back(*vs_it);
			vs_it++;
		}
		it++;
	}
	return (portList);
}

std::list<std::pair<SSL_CTX*, std::string> >	VirtualServers::getSSLPortList()
{
	std::list<std::pair<SSL_CTX*, std::string> >	list;
	virtualServerMap::const_iterator				it = virtualServers.begin();

	while (it != virtualServers.end())
	{
		std::list<std::pair<SSL_CTX*, std::string> >	tmp_list = it->second.getSSLSocketMap();
		std::list<std::pair<SSL_CTX*, std::string> >::const_iterator ittmp = tmp_list.begin();

		while (ittmp != tmp_list.end())
		{
			list.push_back(*ittmp);
			ittmp++;
		}
		it++;
	}
	return (list);
}
