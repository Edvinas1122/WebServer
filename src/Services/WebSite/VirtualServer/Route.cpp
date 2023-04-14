#include <VirtualServer.hpp>

#define METHOD_COUNT 9

const std::string	&httpMethodsList(const unsigned int& iterator)
{
    static const std::string http_methods_str[] = \
		{ "GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH", "" };
	
	if (iterator > METHOD_COUNT)
			return (http_methods_str[10]);
    return (http_methods_str[iterator]);
}

void	Route::ParseAllowedMethods(DescendParser &parser)
{
	size_t	iterator = 0;

	while (METHOD_COUNT > iterator)
	{
		try {
			if (parser.getValue("allow") == httpMethodsList(iterator))
				forbit_methdods[httpMethodsList(iterator)] = false;
		} catch (...) {}
		iterator++;
	}
}

void	Route::ParseForbidMethods(DescendParser &parser)
{
	size_t	iterator = 0;

	while (METHOD_COUNT > iterator)
	{
		try {
			if (parser.getValue("forbid") == httpMethodsList(iterator))
				forbit_methdods[httpMethodsList(iterator)] = true;
		} catch (...) {}
		iterator++;
	}
}

void	Route::SetMethodsDefault()
{
	size_t	iterator = 1;

	forbit_methdods["GET"] = false;
	while (METHOD_COUNT > iterator)
	{
		forbit_methdods[httpMethodsList(iterator)] = true;
		iterator++;
	}
}

Route::Route(DescendParser parser): directory_listing_enabled(false), response_dir(""),
				upload_dir(""), redirect(""), default_file("")
{
	try {
		default_file = parser.getValue("default_file");
	} catch (...) { default_file = ""; }

	try {
		if (parser.getValue("dir_listing") == "enable")
			directory_listing_enabled = true;
		else
			directory_listing_enabled = false;
	} catch (...) { directory_listing_enabled = false; }
	SetMethodsDefault();
	ParseAllowedMethods(parser);
	ParseForbidMethods(parser);

	try {
		upload_dir = parser.getValue("upload_dir");
	} catch (...) { upload_dir = ""; }
	try {
		redirect = parser.getValue("redirect");
	} catch (...) { redirect = ""; }
	try {
		response_dir = parser.getValue("dir_route");
	} catch (...) { response_dir = ""; 
		if (redirect.empty())
			throw RouteHasNoDirNorRedirect();
	}
	
}


void	Route::displayInfo(const char *append) const
{
	std::cout << append << "===" << std::endl;
	std::cout << append << "Response dir: " << response_dir << std::endl;
	std::cout << append << "Upload dir: " << upload_dir << std::endl;
	std::cout << append << "Redirect: " << redirect << std::endl;
	std::cout << append << "Default_file: " << default_file << std::endl;
	std::cout << append << "===" << std::endl;
}