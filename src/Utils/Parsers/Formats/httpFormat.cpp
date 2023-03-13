#include <httpFormat.hpp>

std::map<std::string, int>	getMethodMap()
{
	std::map<std::string, int>	map;

	map["GET"] = GET_METHOD;
	map["HEAD"] = HEAD_METHOD;
	map["POST"] = POST_METHOD;
	map["DELETE"] = DELETE_METHOD;
	map["TRACE"] = TRACE_METHOD;
	map["CONNECT"] = CONNECT_METHOD;
	map["OPTIONS"] = OPTIONS_METHOD;
	return (map);
}

std::map<std::string, int>	getVersionsMap()
{
	std::map<std::string, int>	map;

	map["HTTP/1.1"] = VERSION_1_1;
	map["HTTP/1.0"] = VERSION_1_0;
	return (map);
}


