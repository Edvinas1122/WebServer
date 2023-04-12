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
	if (S_ISDIR(path_stat.st_mode)) {
		return false;
	} else {
		return true;
	}	
}

// enum class FileType { FILE, DIRECTORY, OTHER };

// FileType GetFileType(const std::string& path)
// {
//     struct stat path_stat;
//     if (stat(path.c_str(), &path_stat) != 0)
//     {
// 		throw (std::exception());
//         return FileType::OTHER;
//     }
//     if (S_ISREG(path_stat.st_mode))
//         return FileType::FILE;
//     if (S_ISDIR(path_stat.st_mode))
//         return FileType::DIRECTORY;
//     return FileType::OTHER;
// }

// bool	Access(const std::string &path)
// {
// 	struct stat path_stat;

// 	if (stat(path.c_str(), &path_stat) != 0)
// 		return (false);
// 	if (S_ISREG(path_stat.st_mode) || S_ISDIR(path_stat.st_mode))
// 		return (true);
// 	return (false);
// }

bool	Access(const std::string &path)
{
	int	accessCode = access(path.c_str(), R_OK);
	// ENOENT(accessCode);
	return (!(accessCode));
}

// std::string	HTTPHeaderFileOK(std::string const &path)
// {
// 	std::stringstream	header;

// 	header << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n";
// 	header << "Content-Length: " << File(path.c_str()).GetSize() << "\r\n";
// 	header << "\r\n";
// 	return (header.str());
// }

// std::string	HTTPHeaderDirOK(std::string const &path, const std::string &url)
// {
// 	std::stringstream	header;

// 	header << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n";
// 	header << "Content-Length: " << dirInfoHTTPFormat(path.c_str(), url.c_str(), true).length() << "\r\n";
// 	header << "\r\n";
// 	header << dirInfoHTTPFormat(path.c_str(), url.c_str(), true);
// 	return (header.str());
// }

void	trimUntilFileBegin(Buffer &buffer)
{
	buffer = buffer.substr(buffer.find("Content-Type:"));
	buffer = buffer.substr(buffer.find("\r\n") + 4);
};

Buffer	unchunkBegining(Buffer buffer, std::string const &delimeter)
{
	size_t	begin;

	begin = buffer.find(delimeter);
	if (begin != std::numeric_limits<size_t>::max())
		buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());
	begin = buffer.find(delimeter);
	if (begin != std::numeric_limits<size_t>::max())
		buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());
	return (buffer);
};