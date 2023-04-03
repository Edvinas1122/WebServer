#ifndef UPLOAD_SERVICE_HPP
# define UPLOAD_SERVICE_HPP

# include <includes.hpp>
# include <Client.hpp>
# include <Buffer.hpp>
# include <File.hpp>

class	Upload
{
	private:
		Buffer		previousBuffer;
		Client		*client;
		File		file;
		std::string	chunkDelimeter;

	public:
		Upload() {};
		Upload(const Upload &src) {
			this->client = src.client;
			this->chunkDelimeter = src.chunkDelimeter;
			this->file = src.file;
			this->file.Create();
			previousBuffer.clear();
		};
		virtual	~Upload() {};

		// Upload(Client *client, std::string const &file, std::string const &chunkDelimeter = ""): client(client), file(file.c_str()), chunkDelimeter(chunkDelimeter) {};
		Upload(Client *client, std::string const &chunkDelimeter = ""): client(client), chunkDelimeter(chunkDelimeter) {};

		bool	id(const Client *client) {
			return ((this->client == client));
		};

		void	pullToFile(void (*process)(Client *client, std::string const &delimeter) = NULL)
		{
			if (process != NULL)
			{
				Buffer	tmp;

				if (!previousBuffer.empty()) {
					*client >> tmp;
					*client = previousBuffer;
					*client << tmp;
				}
				process(client, chunkDelimeter);
				*client >> tmp;
				tmp.substr(0, previousBuffer.length());
				*client = tmp;
				*client >> file;
				return;
			}
			if (!previousBuffer.empty())
				previousBuffer >> file;
			*client >> previousBuffer;
		};

		void	setFile(std::string const &filename)
		{
			file.Create(filename.c_str());
		};

		std::string	showDelimiter() const {
			return (chunkDelimeter);
		};

		bool		taskInfo() const
		{
			return (file.ready());
		};
};

class	UploadService
{
	public:
		typedef	std::list<Upload>::iterator	st_iterator;

	private:
		std::list<Upload>	uploadList;

	public:

		void	addClientProcess(Client *client, std::string const &chunkDelimeter = "POST");
		void	Handle(Client *client);

		bool	clientInServiceList(Client *client) {
			return ((checkClientInList(client) != processList.end()));
		};

	private:
		bool	fileEnd(Client *client);
		bool	fileBegin(Client *client);


		void	addDownload(Client *client, std::string const &filedir = "/home/default_file");
		// void	addDownload(Client *client, std::string const &filename = "",
		// 						std::string const &chunkDelimeter = "POST",
		// 						void (*process)(Client *, std::string const &) = NULL);

		void	writeBufferToFile(Client *client, void (*process)(Client *, std::string const &) = NULL);
		void	closeDownload(Client *client);

	private:
		st_iterator	checkClientInList(const Client *client);
};

#endif