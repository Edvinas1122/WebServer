#include <File.hpp>
#include <AttributeGetter.hpp>
#include "/home/WebServer/src/Service/Request/HTTP.hpp"
#include <DescendParser.hpp>
#include <Response.hpp>
#include <Service.hpp>

// void	test_request(HttpRequest &request)
// {
// 	std::cout << request << std::endl;
// 	std::cout << request.getProtocolVersion() << std::endl;
// 	std::cout << request.getMethod() << std::endl;
// 	std::cout << request.getLocation() << std::endl;
// 	std::cout << request.getLocation().getFileName() << std::endl;
// 	std::cout << request.getHost() << std::endl;
// }

int	main(void)
{
	Service			messageProcessor;
	HttpRequest		request;
	File			requestSample("/home/WebServer/samples/http_request.txt");


	messageProcessor.parseConfigurationFile("/home/WebServer/server.conf");
	messageProcessor.Info();
	request.append(requestSample.GetContents());
	return (0);

}