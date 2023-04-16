#include "WebSite.hpp"

static std::string	timestamp()
{
	char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);

	strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
	return (date);
}

const std::string	headerMessage(const int &method_version, const int &code, const size_t content_len, bool closeHeader)
{
	std::stringstream	message;

	message << "HTTP/1." << method_version << " " << code << " " << getHttpExplanation(code) << "\r\n";
	message << "Connection: " << ((content_len != std::numeric_limits<size_t>::max()) ? "keep-alive" : "close") << "\r\n";
	if (content_len != std::numeric_limits<size_t>::max())
		message << "Content-Length: " << content_len << "\r\n";
	message << "Date: " << timestamp() << "\r\n";
	if (closeHeader)
		message << "\r\n";
	return (message.str());
}