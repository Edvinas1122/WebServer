UTILS = $(addprefix src/ServerManager/Utils/, File.cpp AttributeGetter.cpp wordFind.cpp)
PARSER = $(addprefix src/ServerManager/ServerCreator/,DescendParser.cpp DefinitionExtractor.cpp configurationFileFormat.cpp openPortSocket.cpp)
VIRTUAL_SERVER = $(addprefix src/ServerManager/VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
SERVERMANAGER = $(addprefix src/ServerManager/,ServerManager.cpp PortSockets.cpp VirtualServers.cpp ClientQue.cpp Observer.cpp)
CLIENT = $(addprefix src/ServerManager/Client/,Client.cpp Tcp.cpp)
TERMINAL = $(addprefix src/ServerManager/,terminal.cpp)
SRC = src/main.cpp $(SERVERMANAGER) $(VIRTUAL_SERVER) $(PARSER) $(CLIENT) $(TERMINAL) $(UTILS)

HEADER_FILES = -Isrc/ServerManager/ServerCreator/ -Isrc/ServerManager/Utils -Isrc/ 
FLAGS = -Wall -Wextra -Werror -std=c++98 $(HEADER_FILES) -DC98 -g
NAME = server
CC = c++

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm $(NAME)

re: clean all