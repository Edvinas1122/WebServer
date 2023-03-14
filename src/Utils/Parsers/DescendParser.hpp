#ifndef DESCENDPARSER_H
# define DESCENDPARSER_H

# include <includes.hpp>
# include <AttributeGetter.hpp>
# include <DefinitionExtractor.hpp>

/*
	Parser for list structure definitions
	Can be enstructed to scope into object definitions f.e. into curly brackets
	Can also extract mapped definitions - aka pairs
*/

class DescendParser: public AttributeGetter, public DefinitionExtractor
{
	public:
		DescendParser() {};

		void	setContent(std::string const &content) {
			DefinitionExtractor::setDefinitionContent(content);
			AttributeGetter::setObject(content);
		};

		template <typename TYPE>
		TYPE	get(const char *keySpecifier, const int occurance) {
			return (TYPE(descentParser(keySpecifier, occurance)));
		};

		template <typename TYPE>
		std::pair<std::string, TYPE>	getMaped(const char *keySpecifier, const int occurance) {

			return (std::pair<std::string, TYPE>(descentParserByMapedDefinition(keySpecifier, occurance).first,
						TYPE(descentParserByMapedDefinition(keySpecifier, occurance).second)));
		};

	private:

		DescendParser							descentParser(const char *key, const int occurance);
		std::pair<std::string, DescendParser>	descentParserByMapedDefinition(const char *key, const int occurance);

};

#endif