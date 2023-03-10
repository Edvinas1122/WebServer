#include <ServerClasses.hpp>

bool VirtualServer::pathcheck(std::string const &path)
{
	return (access(path.c_str(), R_OK | W_OK));
}

bool VirtualServer::isPositiveNumber(std::string const &str)
{
	std::stringstream ss(str);
	double num;

	if (ss >> num && num > 0 && ss.eof())
		return false;
	else
		return true;
}

bool VirtualServer::validIndexFile(std::string const &str)
{
	(void) str;
	return (false);
}

bool VirtualServer::validServerName(std::string const &str)
{
	(void) str;
	return (false);
}

// bool VirtualServer::validCGI(std::string const &str)
// {
// 	if (str[0] != '.' || str.find('$') == std::string::npos)
// 		return (true);
// 	else
// 		return (false);

// }

// bool VirtualServer::validPath(std::string const &str)
// {
// 	if (str[0] != '/' || str.find('$') == std::string::npos)
// 		return (true);
// 	else
// 		return (false);
// }

// bool VirtualServer::validEnable(std::string const &str)
// {
// 	if (str != "enable" && str != "disable")
// 		return (true);
// 	else
// 		return (false);
// }
