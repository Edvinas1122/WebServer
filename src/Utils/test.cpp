#include "Executor.hpp"

#define	DEFAULT_TMP_OUTPUT_PATH "/home/http/tmp/cgi_tmp_output"
	int	main(void)
	{
		Executor	cgi("/usr/bin/php-cgi");

		cgi.setEnv(5, "REQUEST_METHOD=POST", "REDIRECT_STATUS=200", "CONTENT_LENGTH=17", "CONTENT_TYPE=application/x-www-form-urlencoded", "SCRIPT_FILENAME=/home/WebServer/http/files/index.php");
		cgi.setInputFd(open("/home/WebServer/http/files/HTTPsamples/cgi_post_body.txt", O_RDONLY));
		cgi.executeToFile(1, DEFAULT_TMP_OUTPUT_PATH);
		return (0);

	}