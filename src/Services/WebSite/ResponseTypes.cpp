#include "WebSite.hpp"
#include "mod/contentUtils.hpp"

ServiceProcess	*HTTPParser::ErrorRespone(const int code, bool close_connection)
{
	unsigned int	version = (close_connection ? 0 : 1);

	if ((virtualServer->errorPage(code)).empty())
	{
		if (close_connection)
		{
			theConnection() << headerMessage(version, code) << getHttpExplanation(code);
			return (new TerminateProcess(&theConnection()));
		} else {
			theConnection() << headerMessage(version, code, getHttpExplanation(code).length()) << getHttpExplanation(code);
			return (new HTTPParser(*this));
		}
	} else {
		size_t	fileSize = File(virtualServer->errorPage(code).c_str()).GetSize();
		if (close_connection)
		{
			theConnection() << headerMessage(version, code);
			return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), virtualServer->errorPage(code)));
		} else {
			theConnection() << headerMessage(version, code, fileSize);
			return (new HTTPFileSend(*this, new HTTPParser(*this), virtualServer->errorPage(code)));
		}
	}
}

/*
	GET REQUEST
*/

ServiceProcess	*HTTPParser::handleGetRequest(std::string const &dir, HttpRequest const &request)
{
	if (!Access(dir))
		return (ErrorRespone(404));
	if (!isFile(dir)) { // dirrectory listing handle
		if (virtualServer->dirListingPermited(request.getLocation().getDir()))
		{
			std::string	url_link_to_this_server = std::string("http://") + request.getHostnPort() + request.getLocation();
			std::string	DirList = dirInfoHTTPFormat(dir.c_str(), url_link_to_this_server.c_str(), virtualServer->methodPermited(request.getLocation().getDir(), "POST"));

			theConnection() << headerMessage(1, 200, DirList.length());
			theConnection() << DirList;
			return (new HTTPParser(*this));
		} else
			return (ErrorRespone(200));
	}
	if (virtualServer->isCGI(UrlQuery(dir).getFileExtension()))
	{
		// theConnection() << headerMessage(0, 200, std::numeric_limits<size_t>::max(), false);
		ServiceProcess *process = handleCGIExecution(dir, request);
		process->ScheduleFollowUp(ComposeCGIHeader);
		return (process);
	}
	if (request.getProtocolVersion() == "HTTP/1.0" || request.getKeepAlive() == "close") {
		theConnection() << headerMessage(0, 200);
		return (new HTTPFileSend(*this, new TerminateProcess(&theConnection()), dir));
	}
	theConnection() << headerMessage(1, 200, File(dir.c_str()).GetSize());
	return (new HTTPFileSend(*this, new HTTPParser(*this), dir));
	// theConnection() << headerMessage(1, 200, File(dir.c_str()).GetSize());
	// return (new HTTPFileSend(*this, new HTTPParser(*this), dir));
}

ServiceProcess	*HTTPParser::handleDeleteRequest(std::string const &dir, HttpRequest const &request)
{
	if (!virtualServer->methodPermited(request.getLocation().getDir(), "DELETE"))
		return (ErrorRespone(405));
	if (access(dir.c_str(), W_OK) == 0)
	{
		std::remove(dir.c_str());
		theConnection() << headerMessage(1, 200, 18 + request.getLocation().getFileName().length());
		theConnection() << "Resource Deleted: " << request.getLocation().getFileName();
		return (new HTTPParser(*this));
	}
	return (ErrorRespone(403));
}

static Buffer	removeHeader(std::string const &request)
{
	Buffer	requestBuffered;

	requestBuffered	<< request.substr(request.find("\r\n\r\n") + 4);
	return (requestBuffered);
}


#include <BufferedReceiveTypes.hpp>

static bool	chunkedFileUploadRequest(HttpHeaders const requestHeaders)
{
	if (requestHeaders.find("Transfer-Encoding") != requestHeaders.end())
	{
		if (requestHeaders.at("Transfer-Encoding") == "chunked")
			return (true);
	}
	return (false);
}

void	setContentLen(ServiceProcess *currentProcess, ServiceProcess *following, Connection *connection)
{
	HTTPLenChunkedFileReceive	*received = dynamic_cast<HTTPLenChunkedFileReceive*>(currentProcess);
	ExecuteFile					*executor = dynamic_cast<ExecuteFile*>(following);
	(void) connection;

	if (received && executor)
		executor->SetEnvVariable(setVar("CONTENT_LENGTH", received->getTotalLen()));
	else
		throw std::exception();
}

// std::string	choseUploadDir(std::string const &dir, )

ServiceProcess		*HTTPParser::handleUploadRequest(std::string const &dir, HttpRequest const &request)
{
	HTTPBufferReceive	*process;
	std::string			uploadDir = virtualServer->getUploadDir(request.getLocation().getDir(), request.getLocation().getFileName());

	if (uploadDir.empty())
		uploadDir = dir;
	if (virtualServer->isCGI(UrlQuery(dir).getFileExtension()))
	{
		ExecuteFile		*cgi = handleCGIExecution(dir, request);
		std::string		tmpFile = updateDirIfFileExists(TEMP_FILES_DIR);

		cgi->FileIntoExec(tmpFile);
		cgi->ScheduleFollowUp(ComposeCGIHeader);
		if (chunkedFileUploadRequest(request.getHeaders())) {
			process = new HTTPLenChunkedFileReceive(*this, cgi, tmpFile);
			process->ScheduleFollowUp(setContentLen);
		}
		else {
			size_t	exactFileLen = atol((request.getHeaders().at("Content-Length").c_str()));

			cgi->SetEnvVariable(setVar("CONTENT_LENGTH", to_string(exactFileLen)));
			process = new HTTPFileReceive(*this, cgi, tmpFile, exactFileLen);
		}
	}
	else if (chunkedFileUploadRequest(request.getHeaders())) {
		process = new HTTPLenChunkedFileReceive(*this, new HTTPFileReceiveReport(*this, new TerminateProcess(&theConnection())), updateDirIfFileExists(uploadDir + request.getLocation().getFileName()));
	}
	else {
		size_t	approxFileSize = atol((request.getHeaders().at("Content-Length").c_str()));
		process = new HTTPDelimiterChunkedFileReceive(*this, new HTTPFileReceiveReport(*this, new HTTPParser(*this)), request.getBoundry(), uploadDir, approxFileSize);
	}
	*process << removeHeader(std::string(request));
	return (process);
}
