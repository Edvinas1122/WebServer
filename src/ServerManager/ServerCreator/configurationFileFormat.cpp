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
