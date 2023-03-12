# include <Parser.hpp>

std::string	Parser::getWord(const int n_th, const int row)
{
	std::stringstream	line;
	std::string			token;
	int					iterator;

	token = object_definition;
	iterator = 0;
	while (row > iterator++)
		token = token.substr(token.find('\n', 0), token.length());
	token = token.substr(0, token.find('\n', 0));
	line << token;
	iterator = 0;
	while (n_th + 1 > iterator++)
		line >> token;
	return (token);
}
