// #include "chunkHandlers.hpp"

// void	unchunkEnd(Client *client, std::string const &delimeter)
// {
// 	Buffer	buffer;

// 	*client >> buffer;
// 	buffer = buffer.substr(0, buffer.find(std::string("--") + delimeter));
// 	buffer = buffer.substr(buffer.find_last("\r\n"));
// 	*client = buffer;
// }

// static void	trimUntilFileBegin(Buffer &buffer)
// {
// 	buffer = buffer.substr(buffer.find("Content-Type:"));
// 	buffer = buffer.substr(buffer.find("\r\n") + 4);
// };

// void	unchunkBegining(Client *client, std::string const &delimeter) {
// 	Buffer	buffer;
// 	size_t	begin;

// 	*client >> buffer;
// 	begin = buffer.find(delimeter);
// 	if (begin != std::numeric_limits<size_t>::max())
// 		buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());
// 	begin = buffer.find(delimeter);
// 	if (begin != std::numeric_limits<size_t>::max())
// 		buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());
// 	trimUntilFileBegin(buffer);
// 	*client = buffer;
// };
