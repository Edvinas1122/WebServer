#include <File.hpp>
#include <AttributeGetter.hpp>
#include "/home/WebServer/src/Response/Request/HTTP.hpp"

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
	File		requestSample("/home/WebServer/samples/http_request.txt");
	HttpRequest	request;

	request.append(requestSample.GetContents());


	return (0);

}