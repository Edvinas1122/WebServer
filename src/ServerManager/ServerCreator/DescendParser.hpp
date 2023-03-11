#ifndef DESCENDPARSER_H
# define DESCENDPARSER_H

# include <includes.hpp>
# include <AttributeGetter.hpp>
# include <DefinitionExtractor.hpp>
# include <File.hpp>


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