#include <ServerClasses.hpp>
#include <HttpFormat.hpp>

std::string	MessageProcessor::validateFormat(std::string const &message)
{
	if (validHttpMessage(message))
		return ("you hot \n");
	return ("you cold \n");
}

/*
	Tells if a message been processed
*/
bool	MessageProcessor::processState()
{
	return (!state);
}

// bool	MessageProcessor::validateFormat(bool (*validator)(std::string const &))
// {
// 	return (false);
// }