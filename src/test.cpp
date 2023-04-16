#include <Executor.hpp>


int	main(void)
{
	Executor	cgi("/usr/bin/php-cgi");

	cgi.execute(1, "/home/WebServer/http/files/index.php");
	return (0);

}