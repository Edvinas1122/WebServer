#include <AttributeGetter.hpp>
#include <wordFind.hpp>

std::list<std::string>	AttributeGetter::getValuesList(const char *key_word, bool (*ValueTest)(std::string const&)) EXCEPTION
{
	std::list<std::string>	list_of_ports;
	int						occurance_iterator = 1;

	while (find_word(object_definition, key_word, occurance_iterator) <= object_definition.length() - strlen(key_word))
	{
		list_of_ports.push_back(getValue(key_word, occurance_iterator, ValueTest));
		occurance_iterator++;
	}
	if (occurance_iterator == 1)
		throw NoKeyExcept();
	return (list_of_ports);
}

std::string	AttributeGetter::getValue(const char *key_word, const int occurance, bool (*ValueTest)(std::string const&)) EXCEPTION
{
	std::size_t	value_position;

	value_position = find_word(object_definition, key_word, occurance);
	if (value_position >= object_definition.length() - strlen(key_word))
		throw NoKeyExcept();
	if ((*ValueTest)(matchValue(object_definition, strlen(key_word), value_position)))
		throw ValueExcept();
	return (matchValue(object_definition, strlen(key_word), value_position).c_str());
}

std::pair<std::string, std::string>	AttributeGetter::getPair(const char *key_word, const int occurance,
																bool (*ValueTest)(std::string const&),
																bool (*KeyTest)(std::string const&)) EXCEPTION
{
	std::pair<std::string, std::string>	pair;
	std::size_t	key_position;
	std::size_t	value_position;

	key_position = find_word(object_definition, key_word, occurance);
	if (key_position >= object_definition.length() - strlen(key_word))
		throw NoKeyExcept();
	if ((*KeyTest)(matchKey(object_definition, strlen(key_word), key_position)))
		throw NoKeyExcept();
	pair.first = matchKey(object_definition, strlen(key_word), key_position);
	value_position = object_definition.find(pair.first, key_position) + pair.first.length();
	if ((*ValueTest)(matchValue(object_definition, 0, value_position)))
		throw ValueExcept();
	pair.second = matchValue(object_definition, 0, value_position);
	return (pair);
}

bool AttributeGetter::defaultValidator(std::string const &str)
{
	(void) str;
	return (false);
}

