#ifndef HTTP_FORMAT
# define HTTP_FORMAT

#include <includes.hpp>

/* HTTP METHODS */
# define GET_METHOD 1
# define HEAD_METHOD 2

# define POST_METHOD 3
# define PUT_METHOD 4

# define DELETE_METHOD 5
# define TRACE_METHOD 6

# define CONNECT_METHOD 7
# define OPTIONS_METHOD 8

/* HTTP version */
# define VERSION_1_0 1
# define VERSION_1_1 2


std::map<std::string, int>	validMethodMap();
std::map<std::string, int>	validVersionsMap();

#endif