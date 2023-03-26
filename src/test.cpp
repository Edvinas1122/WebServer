#include <Buffer.hpp>
#include <TCP.hpp>
#include <Client.hpp>

// const char	*trimHeaderOpeningBoundry(std::string string, std::string const &boundry)
// {
// 	if (string.find(boundry) != std::string::npos)
// 	{
// 		string = string.substr(string.find(boundry) + boundry.length());
// 		string = string.substr(string.find(boundry) + boundry.length());
// 	}
// 	return (string.c_str());
// }

// const char	*trimClosingBoundry(std::string string, std::string const &boundry)
// {
// 	if (string.find(boundry) != std::string::npos)
// 		string = string.substr(0, string.find_last_of(boundry));
// 	return (string.c_str());
// }

// void	test_request(HttpRequest &request)
// {
// 	// std::cout << request << std::endl;
// 	// std::cout << request.getProtocolVersion() << std::endl;
// 	// std::cout << request.getMethod() << std::endl;
// 	// std::cout << request.getLocation().getFileName() << std::endl;
// 	// std::cout << request.getLocation().getFileName() << std::endl;
// 	// std::cout << request.getHeaders().at("Content-Type").substr(request.getHeaders().at("Content-Type").find("boundary=") + 9);
// 	// std::cout << removeFileBoundries(request.getHeaders().at("Content-Type").substr(request.getHeaders().at("Content-Type").find("boundary=") + 9))

// }

int	main(void)
{
	Client	copy;
	Buffer	test;

	test.append("content\n", 9);
	copy << test;
	// copy >> test;


	std::cout << copy << std::endl;
	return (0);

}