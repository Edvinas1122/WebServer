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

		void	setObject(std::string const &definition) {
			object_definition = definition;
		};

		template<typename TYPE>
		TYPE	matchMapped(std::map<std::string, TYPE> map, int n_th, int row = 0) {
			return (map[getWord(n_th, row)]);
		};
	
		std::string	getWord(const int n_th, const int row);
	private:

};

// class	defaultProtocol
// {
	
// };

// template <typename TYPE, typename PARSER>
// class	MessageProcessor
// {
// 	private:
// 		bool	state;
// 	public:
// 		MessageProcessor(): state(false) {};

// 		std::string	validateFormat(std::string const &message);

// 		// template<typename TYPE>
// 		TYPE	getParsed()
// 		{
// 			TYPE	protocol(PARSER());

// 			return ();
// 		};

// 		bool		processState()
// 		{
// 			return (!state);
// 		};

// 	private:
		
// };

#endif