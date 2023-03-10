#ifndef ARRAY_HPP
# define ARRAY_HPP

# include <includes.hpp>

template<typename TYPE>
class	Observer: public std::vector<TYPE>
{
	public:
		Observer() {

		};
		~Observer() {};

		void	poll() {
			poll(data(), size(), 10 * 1000);
		};
};

template<typename TYPE>
class	Observer
{
	private:
        std::vector<TYPE>   vector;
		static const int MAX_CONNECTIONS = 10;
		static const int TIMEOUT = 5000;
	public:
		Observer() {
                struct pollfd   empty;
                memset(&empty, 0, sizeof(empty));
                vector.reserve(MAX_CONNECTIONS);
                vector.insert(vector.begin(), MAX_CONNECTIONS, empty);
        };
		~Observer() {};

		int	Poll() {
			int	info;
			info = poll(vector.data(), vector.size(), TIMEOUT);
            return (info);
		};

        TYPE    &operator[](int index) {
            return (vector[index]);
        }
};

#endif