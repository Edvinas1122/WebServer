#ifndef PARSER_HPP
# define PARSER_HPP

#include <includes.hpp>
#include <AttributeGetter.hpp>

class	Parser: public AttributeGetter
{
	public:
		Parser() {};
		Parser(std::string const &definition): AttributeGetter(definition) {};
	
	public:
		template<typename TYPE>
		TYPE	matchMapped(std::map<std::string, TYPE> map, int n_th, int row = 1) {
			return (map[getWord(n_th, row)]);
		};
	
	private:
		std::string	getWord(const int n_th, const int row);

};

#endif