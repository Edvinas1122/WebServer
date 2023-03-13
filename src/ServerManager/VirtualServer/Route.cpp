#include <ServerClasses.hpp>

/*
	Contradicting attributes

	redirect
	default_file

	- after this

	Turn on or off directory listing.
	Set a default file to answer if the request is a directory.
	
*/

// Route::Route(DescendParser parser) 
// {
// 	response_dir = parser.getValue("dir_route");

// }


void	Route::displayInfo(const char *append) const
{
	std::cout << append << "===" << std::endl;
	std::cout << append << "Response dir: " << response_dir << std::endl;
	std::cout << append << "Upload dir: " << upload_dir << std::endl;
	std::cout << append << "Redirect: " << redirect << std::endl;
	std::cout << append << "Default_file: " << default_file << std::endl;
	std::cout << append << "===" << std::endl;
}