#ifndef CONTENT_BROWSER_HPP
# define CONTENT_BROWSER_HPP

# include <includes.hpp>
# include <Service.hpp>
# include <Client.hpp>
# include <Buffer.hpp>
# include <File.hpp>

class	ServiceProcessTrace
{
	private:
		Client		*client;
		File		file;
		std::string	chunkDelimeter;

	public:
		ServiceProcessTrace() {};
		ServiceProcessTrace(const ServiceProcessTrace &src) {
			this->client = src.client;
			this->chunkDelimeter = src.chunkDelimeter;
			this->file = src.file;
			this->file.Create();
		};
		virtual	~ServiceProcessTrace() {};

		ServiceProcessTrace(Client *client, std::string const &file, std::string const &chunkDelimeter = ""): client(client), file(file.c_str()), chunkDelimeter(chunkDelimeter) {};

		bool	id(const Client *client) {
			return ((this->client == client));
		};

		void	pullToFile() {
			*client >> file;
			// file << "test";
		};

		std::string	showDelimiter() const {
			return (chunkDelimeter);
		};
};

class	ChunkHandler
{
	private:
		Buffer		buffer;
	public:
		ChunkHandler() {};
		~ChunkHandler() {};

		bool	chunkEnd(Client *client, std::string const &delimeter) {
			*client >> buffer;
			if (buffer.find(delimeter) != std::numeric_limits<size_t>::max())
				return (true);
			return (false);
		};

	protected:
		void	unchunkBegining(Client *client, std::string const &delimeter) {
			size_t	begin;

			*client >> buffer;

			std::cout << "Chunk delimeter: " << delimeter << std::endl;

			begin = buffer.find(delimeter);
			if (begin != std::numeric_limits<size_t>::max())
				buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());

			std::cout << "Trimmed buffer: " << buffer << std::endl;

			begin = buffer.find(delimeter);

			std::cout << "Begin: " << begin << std::endl;
			
			if (begin != std::numeric_limits<size_t>::max())
				buffer = buffer.substr(begin + delimeter.length(), buffer.length() - begin - delimeter.length());

			*client << buffer;
			buffer.clear();
		};
	
};

class	UploadService: public ChunkHandler
{
	public:
		typedef	std::list<ServiceProcessTrace>::iterator	st_iterator;

	private:
		std::list<ServiceProcessTrace>	processList;

	public:
		void	addDownload(Client *client, std::string const &filename, std::string const &chunkDelimeter = "POST");
		bool	clientInServiceList(Client *client) {
			return ((checkClientInList(client) != processList.end()));
		};
		void	closeDownload(Client *client);
		void	writeBufferToFile(Client *client);
		bool	fileEnd(Client *client) {
			return (ChunkHandler::chunkEnd(client, checkClientInList(client)->showDelimiter()));
		};

	private:
		st_iterator	checkClientInList(const Client *client);
};

# include <HTTP.hpp>

class	ContentBrowser: public Service, public UploadService
{
	public:
		ContentBrowser() {};
		~ContentBrowser() {};

	virtual bool	Ready(Client &client);
	virtual	void	Serve(Client &client);
	virtual	void	Handle(Client &client);
};

#endif
