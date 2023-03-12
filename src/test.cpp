#include <File.hpp>
#include <Parser.hpp>
#include <ServerClasses.hpp>

int	main(void)
{
	File	doc;
	Parser	parser;


	doc.Open("/home/WebServer/samples/http_request.txt");
	parser.setObject(doc.GetContents());
	HttpRequest	type(parser);
	// std::cout << parser.getWord(0, 0) << std::endl;
	return (0);
}