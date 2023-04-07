#ifndef HTTP_MESSAGES_HPP
# define HTTP_MESSAGES_HPP

# include <includes.hpp>

class	UrlQuery: public std::string
{
	private:
		static const char *dir_delim;
		static const char *query_delim;
	public:
		UrlQuery() {};
		UrlQuery(std::string const &fullUrl): std::string(fullUrl) {};

		const std::string	getDir() const;
		const std::string	getFileName() const;
		const std::string	getFileExtension() const;
		const std::string	getQuery() const;
};

class	HttpHeaders: public std::map<std::string, std::string>
{
	public:
		HttpHeaders(const std::string &headers) {
			size_t		begin = 0;

			while (headers.find('\n', begin) != std::string::npos)
			{
				insert(std::pair<std::string, std::string>(headers.substr(begin, headers.find(':', begin) - begin),
					headers.substr(headers.find(':', begin) + 2, headers.find('\n', begin) - headers.find(':', begin) - 3)));
				begin = headers.find('\n', begin) + 1;
			}
		};

};

class	HttpRequest: public std::string
{
	public:
		HttpRequest() {};
		HttpRequest(std::string const &httpMessage): std::string(httpMessage) {};

		bool				Validate() const;
		const std::string	getMethod() const;
		const UrlQuery		getLocation() const;
		const std::string	getProtocolVersion() const;

		const HttpHeaders	getHeaders() const;
		const std::string	getHost() const;
		const std::string	getKeepAlive() const;
		const std::string	getBoundry() const;
		const std::string	getFilename() const;
		const std::string	getBody() const;

		// /*
		// 	Body and File related
		// */
		// const std::string	getContentFileName() const;
};

// class	HttpBody
// {
// 	private:
// 		std::string	string;
// 	public:
// 		HttpBody(HttpRequest request) {
// 			string = request;
// 			if (string.find(request.getBoundry()) != std::string::npos) {
// 				string = string.substr(string.find(request.getBoundry()) + request.getBoundry().length());
// 				string = string.substr(string.find(request.getBoundry()) + request.getBoundry().length());
// 				string = string.substr(string.find('\n') + 1).substr(string.find('\n') + 1);
// 				string = string.substr(string.find('\n') + 1);
// 				string = string.substr(string.find('\n') + 1);
// 				string = string.substr(string.find('\n') + 1);
// 			}
// 		};

// 		const std::string get() const {
// 			return (string);
// 		};
// };

#endif