#ifndef SERVER_INCLUDES_HPP
# define SERVER_INCLUDES_HPP

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <signal.h>
# include <iostream>
# include <string>
# include <map>
# include <sstream>
# include <list>
# include <vector>
# include <poll.h>
# include <fcntl.h>
# include <cerrno>
# include <sys/types.h>
# include <algorithm>
# include <sys/wait.h>
# include <sys/time.h>
# include <fstream>
# include <limits>
# include <dirent.h>

#ifdef C98
# define EXCEPTION throw(std::exception)
// # define EXCEPITON(x) throw(x)
#else
# define EXCEPTION noexcept(false)
// # define EXCEPITON(x) noexcept(false)
#endif

#endif