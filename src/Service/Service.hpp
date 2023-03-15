#ifndef SERVICE_HPP
# define SERVICE_HPP

# include <includes.hpp>
#include <DescendParser.hpp>
#include <Response.hpp>
# include <VirtualServer.hpp>

class	VirtualServers {
	public:
		typedef	std::map<std::string, VirtualServer>	virtualServerMap;
	protected:
		virtualServerMap	virtualServers;
	
	public:
		VirtualServers() {};
		~VirtualServers() {};

		void	Info() const;

		VirtualServer	&getServer(std::string const &port, std::string const &host);

	protected:
		void parseServers(DescendParser &parser)
		{
			int	iterator = 1;

			while (parser.count("server") >= iterator)
			{
				virtualServers[parser.get<VirtualServer>("server", iterator).getHost()] = parser.get<VirtualServer>("server", iterator);
				iterator++;
			};
		}

};

// template<typename REQUEST, typename RESPONSE>
class	Service: public VirtualServers
{
	public:
		Service() {};
		~Service() {};

		void	parseConfigurationFile(const char *path);

		// RESPONSE compose(const REQUEST& request, const std::string &port) {
		// 	// getServer(port, request).root
		// 	return (response(request, getServer(port, request)));
		// }
};

#endif