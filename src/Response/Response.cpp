#include <Response.hpp>
// #include "mod/headers.hpp"

// Response::Response()
// {
// 	_head.clear();
// 	_body = "";
// 	_location = "";
// 	_response = "";
// }

// void Response::build_error(int code)
// {
// 	_status = code;
// 	_body = error::BuildError(code);
// }
// void Response::Build()
// {
// 	setStatusLine();
// 	_response.append(setHeaders());
// 	/*if(reqError || buildBody())
// 		build_error(_status);*/
// 	// _status = 404;
// 	// build_error(_status);
//     // if (_request.getMethod() != HEAD && (_request.getMethod() == GET || _status != 200))
//     //     _response.append(_body);
// }


// bool Response::reqError()
// {
// 	if (_request.getError() == 0)
// 		return (false);
// 	_status = _request.getError();
// 	return (true);
// }

// bool Response::buildBody()
// {
// 	if (_request.getBodylength() > 10000)
// 	{
// 		_status = 413;
// 		return(true);
// 	}
// 	if (handleTarget())
// 		return (true);
// 	if (_request.getMethod() == GET || _request.getMethod() == HEAD)
// 		return(false);
// 	if (_request.getMethod() == POST || _request.getMethod() == PUT)
// 		return(false);
// 	_status = 500;
// 	return(true);
// }

// bool Response::handleTarget()
// {
// 	std::string location_key;
// 	return (false);
// }


// void        Response::setStatusLine()
// {
//     _response.append("HTTP/1.1 " + toString(_status) + " ");
//     _response.append(error::getHttpErrorExplanation(_status));
//     _response.append("\r\n");
// }
