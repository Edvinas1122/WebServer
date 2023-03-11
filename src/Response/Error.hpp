#ifndef ERROR_HPP
# define ERROR_HPP
#include <string>
#include <sstream>
#define errorbody1 "<!DOCTYPE html><html> <head> <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"> <title>400</title> <style>@import url('https://fonts.googleapis.com/css?family=Press Start 2P'); html, body{width: 100%; height: 100%; margin: 0;}*{font-family: 'Press Start 2P', cursive; box-sizing: border-box;}#app{padding: 1rem; background: black; display: flex; height: 100%; justify-content: center; align-items: center; color: #54FE55; text-shadow: 0px 0px 10px; font-size: 6rem; flex-direction: column;}#app .txt{font-size: 1.8rem;}@keyframes blink{0%{opacity: 0;}49%{opacity: 0;}50%{opacity: 1;}100%{opacity: 1;}}.blink{animation-name: blink; animation-duration: 1s; animation-iteration-count: infinite;}</style> </head> <body> <div id=\"app\"> <div>"
#define errorbody2 "</div><div class=\"txt\"> "
#define errorbody3 "<span class=\"blink\">_</span> </div></div></body></html>"


template <typename T>
std::string toString(const T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

class error
{
	private:
	public:
		static std::string getHttpErrorExplanation(const int code);
		static std::string BuildError(const int code);

};
#endif // ERROR_HPP