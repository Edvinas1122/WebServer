#include <Response.hpp>
#include <httpFormat.hpp>

HttpRequest::HttpRequest(Parser parser)
{
	std::cout << parser.matchMapped(getMethodMap(), 0) << std::endl;
	std::cout << parser.getWord(1, 0) << std::endl;
	std::cout << parser.matchMapped(getVersionsMap(), 2) << std::endl;
}