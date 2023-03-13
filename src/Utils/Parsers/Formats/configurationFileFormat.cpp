#include <DescendParser.hpp>
#include <configurationFileFormat.hpp>

std::string	removeComents(std::string const &content)
{
	std::stringstream	stream(content);
	std::string			line;
	std::string			result;
	std::size_t			comment_pos;
  
	while (std::getline(stream, line)) {
		comment_pos = line.find('#');
		if (comment_pos != std::string::npos) {
			line.erase(comment_pos);
		}
		result += line + '\n';
	}
	return (result);
}

std::list<std::string>	wordMatchMethod(DescendParser &parser)
{
	std::list<std::string>	portSockets;
	int	iterator = 1;

	while (parser.count("listen") >= iterator)
	{
		if (std::find(portSockets.begin(), portSockets.end(), parser.getValue("listen", iterator).c_str()) == portSockets.end())
		{
			std::cout << parser.getValue("listen", iterator).c_str() << std::endl;
			portSockets.push_back(parser.getValue("listen", iterator));
		}
		iterator++;
	}
	return (portSockets);
}