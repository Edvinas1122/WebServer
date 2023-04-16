#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

static std::string	getRemainer(char *buffer, size_t start, size_t const len)
{
	std::string	remainer;

	while (start < len)
	{
		remainer.push_back(buffer[start]);
		start++;	
	}
	return (remainer);
}

static std::string	getConententType(std::string const &params)
{
	return (params.substr(params.find("Content-type: "), params.find("\r\n\r\n")));
}

static size_t	getStatus(std::string &params)
{
	std::string status;
	size_t		begin;

	begin = params.find("Status: ");
	status = params.substr(begin, params.find("\r\n", begin));
	return (atol(status.c_str()));
}

std::string	vectorToStr(std::vector<char> *array, size_t const len)
{
	std::string	data;
	size_t		iterator = 0;

	while (iterator < len)
	{
		data.push_back((char)(*array)[iterator]);
		iterator++;
	}
	return (data);
}

std::string	getBodyParams(FileSend *send, std::vector<char> *array)
{
	char	*mem_pos;
	size_t	position;
	size_t	iterator = 1;
	char	buffer[50];

	while (array->size() < 200)
	{
		send->GetBuffer(buffer, 50);
		array->insert(array->end(), buffer, buffer + 50);
		mem_pos = (char*)memmem((const void*)array->data(), 50 * iterator, (const void*)"\r\n\r\n", 4);
		if (mem_pos)
		{
			position = (char*)memmem((const void*)buffer, 50, (const void*)"\r\n\r\n", 4) - buffer;
			return (vectorToStr(array, position));
		}
		iterator++;
	}
	return ("");
	// throw std::exception();
}

void	ComposeCGIHeader(ServiceProcess *currentProcess, ServiceProcess *following, Connection *connection)
{
	FileSend			*send = dynamic_cast<FileSend*>(following);
	ExecuteFile			*executor = dynamic_cast<ExecuteFile*>(currentProcess);
	std::string			params;
	std::vector<char>	array;

	array.reserve(200);
	if (send && executor) {
		(void)executor;
		params = getBodyParams(send, &array);
		if (params.find("Status:") != std::string::npos)
			*connection << headerMessage(0, getStatus(params));
		else
			*connection << headerMessage(0, 200, std::numeric_limits<size_t>::max(), false);
		*connection << getConententType(params);
		*connection << getRemainer(array.data(), params.length(), 50);
	}
	else
		throw std::exception();
}

ExecuteFile	*HTTPParser::handleCGIExecution(std::string const &dir, HttpRequest const &request)
{
	std::string	cgiExecutableDir = virtualServer->CGIexecutableDir(UrlQuery(dir).getFileExtension());

	// ExecuteFile	*exec = new ExecuteFile(&theConnection(), new HTTPParser(*this), cgiExecutableDir, dir);
	ExecuteFile	*exec = new ExecuteFile(&theConnection(), new TerminateProcess(&theConnection()), cgiExecutableDir, dir);
	//critical
	exec->SetEnvVariable(setVar("REQUEST_METHOD", request.getMethod()));
	exec->SetEnvVariable(setVar("REDIRECT_STATUS", "200"));
	exec->SetEnvVariable(setVar("SCRIPT_FILENAME", dir));
	try {
		exec->SetEnvVariable(setVar("CONTENT_TYPE", request.getHeaders().at("Content-Type")));
	} catch (...) {}
	try {
		exec->SetEnvVariable(setVar("CONTENT_LENGTH", request.getHeaders().at("Content-Length")));
	} catch (...) {}
	//non-critical
	exec->SetEnvVariable(setVar("QUERY_STRING", request.getLocation().getQuery()));
	exec->SetEnvVariable(setVar("SCRIPT_NAME", request.getLocation().getFileName()));
	exec->SetEnvVariable(setVar("PATH_INFO", (request.getLocation().getCGIPathInfo().empty()) ? "" : std::string("/") + request.getLocation().getCGIPathInfo()));
	exec->SetEnvVariable(setVar("SERVER_PROTOCOL", request.getProtocolVersion()));
	exec->SetEnvVariable(setVar("REQUEST_URI", request.getLocation()));
	exec->OutputToFile(updateDirIfFileExists(TEMP_FILES_DIR));
	return (exec);
}

