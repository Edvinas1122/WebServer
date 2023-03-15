#include <Service.hpp>
#include "/home/WebServer/src/Utils/Parsers/Formats/configurationFileFormat.hpp"
#include <File.hpp>

// template<typename REQUEST, typename RESPONSE>
void	Service::parseConfigurationFile(const char *path)
{
	DescendParser        parser;
	FileProcessor        configurationFile;

	configurationFile.Open(path);
	parser.setContent(configurationFile.GetContents(removeComents));
	parseServers(parser);
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

VirtualServer	&VirtualServers::getServer(std::string const &port, std::string const &host)
{
	(void) port;
	return ((virtualServers[host]));
}