#include <HttpFormat.hpp>

bool	validHttpMessage(std::string const &message)
{
	if (message.find("Dog") != std::string::npos)
		return (true);
	return (false);
}

