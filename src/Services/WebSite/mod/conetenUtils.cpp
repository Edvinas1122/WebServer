#include "contentUtils.hpp"

bool	isFile(const std::string &path)
{
	struct stat path_stat;

	if (stat(path.c_str(), &path_stat) != 0) {
		// Error accessing path
		return false;
	}
	if (S_ISREG(path_stat.st_mode)) {
		return true;
	} else {
		return false;
	}	
}

bool	isDir(const std::string &path)
{
	struct stat path_stat;

	if (stat(path.c_str(), &path_stat) != 0) {
		// Error accessing path
		return false;
	}
	if (S_ISDIR(path_stat.st_mode)) {
		return true;
	} else {
		return false;
	}	
}


bool	Access(const std::string &path)
{
	int	accessCode = access(path.c_str(), R_OK);
	// ENOENT(accessCode);
	return (!(accessCode));
}

std::string	to_string(size_t const number)
{
	std::stringstream	str;

	str << number;
	return (str.str());
}

static std::string	addAppendixToFileName(std::string const &dir, size_t appendix)
{
	if (dir.find_last_of('.') != std::string::npos)	
		return (dir.substr(0, dir.find_last_of('.')) + "_" + to_string(appendix) + dir.substr(dir.find_last_of('.')));
	else
		return (dir + "_" + to_string(appendix));
}

std::string	updateDirIfFileExists(std::string const &dir)
{
	size_t				appendix = 1;
	std::string			altered;

	if (!isFile(dir))
		return (dir);

	while (isFile(addAppendixToFileName(dir, appendix)))
		appendix++;
	return (addAppendixToFileName(dir, appendix));
}

const std::string	fixDir(std::string const &urlDir)
{
	std::string	dirFixed;

	if (urlDir.empty())
		dirFixed = "/";
	else if (urlDir.at(urlDir.length() - 1) != '/' && urlDir.at(0) != '/')
		dirFixed = std::string("/") + urlDir + "/"; // not recognized by http parser as urldir
	else if (urlDir.at(urlDir.length() - 1) != '/')
		dirFixed = urlDir + "/";
	else
		dirFixed = std::string("/") + urlDir;
	return (dirFixed);
}

const std::string	setVar(std::string const &key, std::string const &value)
{
	if (value.empty())
		return (key);
	return (key + "=" + value);
}