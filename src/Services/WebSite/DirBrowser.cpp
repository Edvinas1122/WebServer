#include "WebSite.hpp"

static std::string	typeInfo(std::string const &path)
{
	struct stat	info;

	if (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
		return ("Dir: ");
	else
		return ("File: ");
}

static std::string	DeleteForm(std::string const &sysPath, std::string const &path)
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

static std::string	UploadForm(std::string const &path)
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

const std::string	dirInfoHTTPFormat(const char *path, std::string const &url, bool displayUpload)
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