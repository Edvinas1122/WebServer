#include <wordFind.hpp>

static int	is_value_symbol(const char c)
{
	if ((c > 32 && c < 91) || (c > 94 && c <= 122))
		return (1);
	return (0);
}

static int	check_allowed_neighbours(char c)
{
	if (c == ' ' || c == ';' || c == '{' || c == '\n' || c == '\t')
		return (HAS_ALLOWED);
	return (-1);
}

static int	check_match_word(std::string const &content, std::size_t index, std::size_t word_len)
{
	if (index != 0){
		if (check_allowed_neighbours(content[index - 1]) != HAS_ALLOWED)
			return (-1);
	}
	if (check_allowed_neighbours(content[index + word_len]) != HAS_ALLOWED)
		return (-1);
	return (MATCH);
}

std::size_t	find_word(std::string const &content, const char *word_to_match, const std::size_t wanted_occurance)
{
	std::size_t test_occur_index = 0;
	std::size_t occurance = 0;

	while (test_occur_index < content.length())
	{
		test_occur_index = content.find(word_to_match, test_occur_index);
		if (test_occur_index == std::string::npos)
			return (content.length() - 1);
		if (check_match_word(content, test_occur_index, strlen(word_to_match)) == MATCH)
			occurance++;
		if (occurance == wanted_occurance)
			return (test_occur_index);
		test_occur_index++;
	}
	return (test_occur_index);
}

std::string	matchValue(std::string const &content, const int key_len, const int index, const char terminator) EXCEPTION
{
	std::size_t	iterator = index + key_len;
	std::size_t	begin = 0;
	std::size_t	end = 0;

	while (content[iterator] != terminator && content[iterator])
	{
		if (!begin && is_value_symbol(content[iterator]))
			begin = iterator;
		if (begin)
		{
			if (end && is_value_symbol(content[iterator]))
				throw TerminatorFailure();
			if ((!end && !is_value_symbol(content[iterator]))) {
				end = iterator;
			}
		}
		iterator++;
	}
	if (!end && content[iterator] == terminator)
		end = iterator;
	return (content.substr(begin, end - begin));
}

std::string	matchKey(std::string const &content, const int key_len, const int index, const char terminator) EXCEPTION
{
	std::size_t	iterator = index + key_len;
	std::size_t	begin = 0;
	std::size_t	end = 0;

	while (content[iterator])
	{
		if (!begin && is_value_symbol(content[iterator]))
			begin = iterator;
		if (begin)
		{
			if (end && is_value_symbol(content[iterator]))
				throw TerminatorFailure();
			if ((!end && !is_value_symbol(content[iterator]))) {
				end = iterator;
				break;
			}
		}
		iterator++;
	}
	if (!end && content[iterator] == terminator)
		end = iterator;
	return (content.substr(begin, end - begin));
}