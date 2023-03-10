#include <DescendParser.hpp>

std::pair<std::string, DescendParser>	DescendParser::descentParserByMapedDefinition(const char *key, const int occurance)
{
	std::pair<std::string, DescendParser>	mapedParser;

	mapedParser.first = getMapedDefinition(key, occurance).first;
	mapedParser.second.setObject(getMapedDefinition(key, occurance).second);
	mapedParser.second.setDefinitionContent(getMapedDefinition(key, occurance).second);
	return (mapedParser);
}

DescendParser	DescendParser::descentParser(const char *key, const int occurance)
{
	DescendParser	parser;

	parser.setObject(getDefinition(key, occurance));
	parser.setDefinitionContent(getDefinition(key, occurance));

	return (parser);
}