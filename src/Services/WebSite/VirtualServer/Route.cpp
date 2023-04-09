#include <VirtualServer.hpp>

/*
	Contradicting attributes

	redirect
	default_file

	- after this

	Turn on or off directory listing.
	Set a default file to answer if the request is a directory.
	
*/

Route::Route(DescendParser parser): directory_listing_enabled(true), response_dir(""),
				upload_dir(""), redirect(""), default_file("")
{
	try {
		response_dir = parser.getValue("dir_route");
	} catch (...) { response_dir = ""; }
	
	try {
		default_file = parser.getValue("default_file");
	} catch (...) { default_file = ""; }

	try {
		if (parser.getValue("dir_listing") == "enable")
			directory_listing_enabled = true;
		else
			directory_listing_enabled = false;
	} catch (...) { directory_listing_enabled = true; }
	try {
		if (parser.getValue("forbid") == "GET")
			forbit_methods[0] = true;
	} catch (...) { forbit_methods[0] = false; }

	try {
		upload_dir = parser.getValue("upload_dir");
	} catch (...) { upload_dir = ""; }
	try {
		redirect = parser.getValue("redirect");
	} catch (...) { upload_dir = ""; }
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