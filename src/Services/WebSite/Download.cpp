#include <WebSite.hpp>

bool	Download::Ready()
{
	if (file.GetSize())
		return (true);
	return (false);
}

/*
	Que a buffer for delivery
*/
void	Download::Handle()
{
	*client << file;
}