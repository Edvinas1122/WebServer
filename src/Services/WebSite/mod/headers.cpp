#include "headers.hpp"



std::string	setHeaders()
{
	char date[1000];
	std::string	header;
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    

	header.append("Content-Type: text/html\r\n");
    // header.append("Content-Length: ");
    // header.append(toString(_body.length()));
    // header.append("\r\n");
    //connection();
    //server();
    //location();
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    header.append("Date: ");
    header.append(date);
    header.append("\r\n");
    header.append("\r\n");
	return (header);
}