#include "contentUtils.hpp"

std::string	typeInfo(std::string const &path)
{
	struct stat	info;

	if (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
		return ("Dir: ");
	else
		return ("File: ");
}

std::string	DeleteForm(std::string const &sysPath, std::string const &path)
{
	std::string	form;
	struct stat	info;

	stat(sysPath.c_str(), &info);
	if ((access(sysPath.c_str(), W_OK) != 0)|| S_ISDIR(info.st_mode))
		return ("");
	form.append("<div><form action=\"");
	form.append(path);
	form.append("\" method=\"POST\">");
	form.append("<input type=\"hidden\" name=\"_method\" value=\"DELETE\">");
	form.append("<input type=\"submit\" value=\"Delete Resource\"></form></div>");
	return (form);
}

std::string	UploadForm(std::string const &path)
{
	std::string	form;

	form.append("<div><form action=\"");
	form.append(path);
	if (path.at(path.length() - 1) != '/')
		form.append("/");
	form.append("\" method=\"post\" enctype=\"multipart/form-data\">");
	form.append("<text>Upload</text>");
	form.append("<input type=\"file\" name=\"fileToUpload\">");
	form.append("<input type=\"submit\" value=\"Upload\" name=\"submit\">");
	form.append("</form></div>");
	return (form);
}

std::string	dirInfoHTTPFormat(const char *path, std::string const &url, bool displayUpload)
{
	DIR				*dir_ptr;
	struct dirent	*entry;
	std::string		info;

	dir_ptr = opendir(path);
	if (dir_ptr == NULL)
		throw std::exception();
	info.append("<head></head>");
	info.append("<body><div>");
	while (42)
	{
		std::string	fullUrl;
		entry = readdir(dir_ptr);
		if (entry == NULL)
			break;
		info.append("<div><a href=\"");
		fullUrl = url;
		if (url.at(url.length() - 1) != '/')
			fullUrl.append("/");
		fullUrl.append(entry->d_name);
		info.append(fullUrl);
		info.append("\">");
		info.append(typeInfo(std::string(path) + std::string("/") + std::string(entry->d_name)));
		info.append(entry->d_name);
		info.append("</a></div>");
		if (displayUpload && std::string(entry->d_name) != ".." && std::string(entry->d_name) != ".")
			info.append(DeleteForm(std::string(path) + std::string("/") + std::string(entry->d_name), fullUrl));
		fullUrl.clear();
	}
	info.append("</div>");
	if (displayUpload) {
		info.append(UploadForm(url.c_str()));
	}
	info.append("</body>");
	closedir(dir_ptr);
	return (info);
}

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

const std::string	headerMessage(const int &method_version, const int &code, const size_t content_len, bool closeHeader)
{
	std::stringstream	message;

	message << "HTTP/1." << method_version << " " << code << " " << getHttpExplanation(code) << "\r\n";
	message << "Connection: " << ((content_len != std::numeric_limits<size_t>::max()) ? "keep-alive" : "close") << "\r\n";
	if (content_len != std::numeric_limits<size_t>::max())
		message << "Content-Length: " << content_len << "\r\n";
	// message << "Date: ";
	if (closeHeader)
		message << "\r\n";
	return (message.str());
}