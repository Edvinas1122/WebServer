#include <VirtualServer.hpp>

bool VirtualServer::pathcheck(std::string const &path)
{
	return (access(path.c_str(), R_OK));
}

bool VirtualServer::isPositiveNumber(std::string const &str)
{
	std::stringstream ss(str);
	int	num;

	ss >> num;
	if (num > 0)
		return (true);
	else
		return (false);
}

bool VirtualServer::isPositiveNumberInErrorRange(std::string const &str)
{
	std::stringstream ss(str);
	int	num;

	ss >> num;
	if (num > 0)
	{
		if (num > 500)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool VirtualServer::validIndexFile(std::string const &str)
{
	(void) str;
	return (false);
}

bool VirtualServer::validServerName(std::string const &str)
{
	(void) str;
	return (true);
}

bool VirtualServer::validCGI(std::string const &str)
{
	if (str[0] == '.')
		return (true);
	else
		return (false);

}

bool VirtualServer::validPath(std::string const &str)
{
	if (str[0] == '/')
		return (true);
	else
		return (false);
}

// bool VirtualServer::validEnable(std::string const &str)
// {
// 	if (str != "enable" && str != "disable")
// 		return (true);
// 	else
// 		return (false);
// }
