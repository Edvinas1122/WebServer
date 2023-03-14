#ifndef PARSER_HPP
# define PARSER_HPP

#include <includes.hpp>
#include <AttributeGetter.hpp>

/*
	Used to get types that supports this parser
	a.d. for objects that can use this interface to aquire own variables

	example:
		template <typename PARSER>
		HttpRequest(PARSER parser);
*/

class	Parser: public AttributeGetter
{
	public:
		Parser() {};
		Parser(std::string const &definition): AttributeGetter(definition) {};
	
	public:

		void	setObject(std::string const &definition) {
			AttributeGetter::setObject(definition);
		};

		template <typename TYPE>
		TYPE	get() {
			return (TYPE(*this));
		};
};

#endif