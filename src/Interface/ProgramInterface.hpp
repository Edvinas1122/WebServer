#ifndef PROGRAM_INTERFACE_HPP
# define PROGRAM_INTERFACE_HPP

# include <includes.hpp>

class	ProgramInterface
{
	public:
		typedef std::map<std::string, std::list<std::string> >	SysCommandQue;
		typedef std::map<std::string, SysCommandQue> SysMessageQueMap;
	protected:
		static bool				run;
		static SysMessageQueMap	data;
	public:
		ProgramInterface() {};
		virtual ~ProgramInterface() {};

	virtual void		Input() = 0;
	virtual bool		EndProgram() {
		return (!run);
	};
	// virtual bool	DataFeed(std::string const &match) {
	// 	std::string	outside = data;
	// 	data.clear();
	// 	return (outside);
	// };

	void	addSystemMessage(std::string const &operator_id, std::string const &info_type, std::string const &message)
	{
		data[operator_id][info_type].push_back(message);
	}

	protected:

	std::string	DataFeed(std::string const &id, std::string const &info_type)
	{
		if (!data.empty() && data.find(id) != data.end() && data.at(id).find(info_type) != data.at(id).end())
		{
			std::list<std::string>	outside = data.at(id).at(info_type);
			std::list<std::string>::const_iterator	it = outside.begin();
			std::string		dataReturn;
			
			while (it != outside.end())
			{
				dataReturn.append(*it);
				it++;
			}
			data[id].clear();
			return (dataReturn);
		}
		return ("");
	};


	void	Terminate() {run = false;};
};

// class	NetworkControll : public ProgramInterface
// {
// 	private:
// 		std::string	data;
// 	public:
// 		NetworkControll() {};
// 		~NetworkControll() {};
	
// 	void	Input();
// }

class	Terminal : public ProgramInterface
{
	public:
		Terminal(): ProgramInterface(), available(true) {};
		virtual ~Terminal() {};

	void	Input();
	std::string	DataGet(std::string const &info_type) {
		std::string	data = ProgramInterface::DataFeed("Terminal", info_type);

		if (!data.empty())
		{
			if (data.find("background")) {
				available = false;
			}
		}
		return (data);
	};

	private:
	void	Parse(std::string const &user_input);
	bool	available;
};

#endif