#ifndef PROGRAM_INTERFACE_HPP
# define PROGRAM_INTERFACE_HPP

# include <includes.hpp>

class	ProgramInterface
{
	public:
		typedef std::map<std::string, std::map<std::string, std::string> > SysMessageQueMap;
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

	protected:

	virtual std::string	DataFeed(std::string const &id, std::string const &info_type)
	{
		std::map<std::string, std::string>	outside = data[id];

		data[id].clear();
		if (info_type.empty())
			return (outside.begin()->second);
		if (outside.find(info_type) != outside.end())
			return (outside.find(info_type)->second);
		return ("");
	};

	void	addSystemMessage(std::string const &operator_id, std::string const &info_type, std::string const &message)
	{
		data[operator_id][info_type] = message;
	}

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
		Terminal(): ProgramInterface() {};
		virtual ~Terminal() {};

	void	Input();

	private:
	void	Parse(std::string const &user_input);
};

#endif