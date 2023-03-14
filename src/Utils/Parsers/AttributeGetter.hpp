#ifndef ATTRIBUTE_GETTER_HPP
# define ATTRIBUTE_GETTER_HPP

# include <includes.hpp>
#define SUCESS 0
#define FAILED_SOCKET -1
#define LIST_END NULL
#define HAS_ALLOWED 0
#define MATCH 0

/*
	An interface that provides tools for classes to parse
	attributes from text file
*/

class	AttributeGetter
{
	private:
		std::string	object_definition;
	protected:
		AttributeGetter() {};
		AttributeGetter(std::string const &definition): object_definition(definition) {};
	
		void	setObject(std::string const &definition) {
			object_definition = definition;
		};
	public:
		/* Atribure parcers */
		std::list<std::string>	getValuesList(const char *key_word,
															bool (*ValueTest)(std::string const&) = defaultValidator) const EXCEPTION;

		std::string	getValue(const char *key_word,
						const int occurance = 1, bool (*ValueTest)(std::string const&) = defaultValidator) const EXCEPTION;

		std::pair<std::string, std::string>	getPair(const char *key_word,
						const int occurance = 1, bool (*ValueTest)(std::string const&) = defaultValidator,
												bool (*KeyTest)(std::string const&) = defaultValidator) const EXCEPTION;

		std::string	getWord(const int n_th, const int row = 0, bool (*ValueTest)(std::string const&) = defaultValidator) const EXCEPTION;

		std::string	findSubstring(const char* startChars, const char* endChars) const;

		template<typename TYPE>
		TYPE	matchMapped(std::map<std::string, TYPE> map, int n_th, int row = 0) {
			if (map.at(getWord(n_th, row)) == map.end())
				throw ValueExcept();
			return (map[getWord(n_th, row)]);
		};

	public:
		class	ValidationFailure: public std::exception {};
			class	SemiCollonFailure: public ValidationFailure {};
			class	NoKeyExcept: public ValidationFailure {};
			class	ValueExcept: public ValidationFailure {};
	
		static bool	defaultValidator(std::string const &str);
};

#endif