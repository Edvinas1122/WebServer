#ifndef WORD_FIND_HPP
# define WORD_FIND_HPP

# include <includes.hpp>

#define SUCESS 0
#define FAILED_SOCKET -1
#define LIST_END NULL
#define HAS_ALLOWED 0
#define MATCH 0

class	TerminatorFailure: public std::exception {};

std::size_t	find_word(std::string const &content, const char *word_to_match, const std::size_t wanted_occurance);
std::string	matchValue(std::string const &content, const int key_len, const int index, const char terminator = ';') EXCEPTION;
std::string	matchKey(std::string const &content, const int key_len, const int index, const char terminator = ' ') EXCEPTION;

#endif
