#ifndef DEFINITION_EXTRACTOR_HPP
# define DEFINITION_EXTRACTOR_HPP

# include <includes.hpp>

/*
	definition extract scope brackets fix
*/
class DefinitionExtractor
{
	private:
		std::string	definitionContent;
	protected:
		DefinitionExtractor() {};
		DefinitionExtractor(const std::string &definitionContent): definitionContent(definitionContent) {};
		~DefinitionExtractor() {};

		void		setDefinitionContent(const std::string &content) {
			definitionContent = content;
		};
		std::string	getDefinition(const char *key, const int occurance) EXCEPTION;
		std::pair<std::string, std::string> getMapedDefinition(const char *key, const int occurance) EXCEPTION;

	public:
		int	count(const char *key);

	private:
		/* Utils */
		static std::size_t	findEnclosingBracket(std::string const &content, const std::size_t start_at);

	public:
		class	ValidationFailure: public std::exception {};
			class	EnclosureFailure: public ValidationFailure {};
			class	NoKeyFailure: public ValidationFailure {};

};

#endif