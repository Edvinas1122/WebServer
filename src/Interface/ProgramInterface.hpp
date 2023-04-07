#ifndef PROGRAM_INTERFACE_HPP
# define PROGRAM_INTERFACE_HPP

# include <includes.hpp>

class	ProgramInterface
{
	protected:
		static bool			run;
		static std::string	data;
	public:
		ProgramInterface() {};
		virtual ~ProgramInterface() {};

	virtual void		Input() = 0;
	virtual bool		EndProgram() {
		return (!run);
	};
	virtual std::string	DataFeed() {
		std::string	outside = data;
		data.clear();
		return (outside);
	};

	protected:

	void	Terminate() {run = false;};
};

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