#include "HTTP.hpp"

int	main(void)
{
	UrlQuery	link("/docker/index.php?dogs=funny");

	std::cout << link.getDir() << std::endl;
	std::cout << link.getFileName() << std::endl;
	std::cout << link.getFileExtension() << std::endl;
	std::cout << link.getQuery() << std::endl;


	
	// doc.Open("/home/WebServer/samples/http_request.txt");
	// parser.setObject(doc.GetContents());
	// parser.get<HttpRequest>();
	// std::cout << parser.getWord(0, 0) << std::endl;
	return (0);
}