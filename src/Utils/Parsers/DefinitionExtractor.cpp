#include <DefinitionExtractor.hpp>
#include "mod/wordFind.hpp"


std::pair<std::string, std::string> DefinitionExtractor::getMapedDefinition(const char *key, const int occurance) EXCEPTION
{
	std::pair<std::string, std::string>	route_definition_text;
	std::size_t begin_index_of_key;
	std::size_t index_of_closing_bracket;

	begin_index_of_key = find_word(definitionContent, key, occurance);
	if (begin_index_of_key >= (definitionContent.length() - strlen(key)))
		return (std::pair<std::string, std::string>("", ""));
	route_definition_text.first = matchValue(definitionContent, strlen(key), begin_index_of_key, '{');
	begin_index_of_key = begin_index_of_key + route_definition_text.first.length();
	index_of_closing_bracket = findEnclosingBracket(definitionContent, begin_index_of_key);
	if (!index_of_closing_bracket)
		throw EnclosureFailure();
	begin_index_of_key = definitionContent.find("{", begin_index_of_key) + 1;
	route_definition_text.second = definitionContent.substr(begin_index_of_key, index_of_closing_bracket - begin_index_of_key - 1);
	return (route_definition_text);
}

std::string DefinitionExtractor::getDefinition(const char *key, const int occurance) EXCEPTION
{
	std::size_t first_server_pos;
	std::size_t second_server_pos;

	first_server_pos = find_word(definitionContent, key, occurance);
	if (first_server_pos >= (definitionContent.length() - strlen(key)))
		return ("");
	second_server_pos = findEnclosingBracket(definitionContent, first_server_pos);
	if (!second_server_pos)
		throw EnclosureFailure();
	first_server_pos = definitionContent.find("{", first_server_pos) + 1;
	return (definitionContent.substr(first_server_pos, second_server_pos - first_server_pos - 1));
}

int	DefinitionExtractor::count(const char *key)
{
	int	count = 0;

	while (42)
	{
		if (definitionContent.length() < find_word(definitionContent, key, count + 1) + strlen(key))
			break;
		count++;
	}
	return (count);
}

/*
	Utils
*/

std::size_t	DefinitionExtractor::findEnclosingBracket(std::string const &content, const std::size_t start_at)
{
	std::size_t	index = 0;
	int			bracket_count = 0;

	index = content.find("{", start_at);
	while (content[index])
	{
		if (content[index] == '{')
			bracket_count++;
		else if (content[index] == '}')
			bracket_count--;
		if (!bracket_count)
			return (index);
		index++;
	}
	return (0);
}
