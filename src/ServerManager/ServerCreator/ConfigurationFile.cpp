#include <ConfigurationFile.hpp>

void	ConfigurationFile::accessCheck() EXCEPTION
{
	if (access(path, R_OK))
		throw ConfFileAccessFailure();
}

void	ConfigurationFile::loadFile() EXCEPTION
{
	std::ifstream file(path);

	content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void	ConfigurationFile::removeComents()
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
	content = result;
}

std::string	ConfigurationFile::getContent()
{
	accessCheck();
	loadFile();
	removeComents();
	return (content);
}