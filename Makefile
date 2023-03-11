UTILS = $(addprefix src/ServerManager/Utils/, File.cpp AttributeGetter.cpp Parser.cpp wordFind.cpp)
PARSER = $(addprefix src/ServerManager/ServerCreator/,DescendParser.cpp DefinitionExtractor.cpp configurationFileFormat.cpp openPortSocket.cpp)
VIRTUAL_SERVER = $(addprefix src/ServerManager/VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
SERVERMANAGER = $(addprefix src/ServerManager/,ServerManager.cpp PortSockets.cpp VirtualServers.cpp ClientQue.cpp Observer.cpp)
HTTP_PROTOCOL = $(addprefix src/ServerManager/HTTP_Protocol/,HttpFormat.cpp)
CLIENT = $(addprefix src/ServerManager/Client/,Client.cpp Tcp.cpp MessageProcessor.cpp)
TERMINAL = $(addprefix src/ServerManager/,terminal.cpp)
Response = $(addprefix src/Response/,Error.cpp Response.cpp)
SRC = src/main.cpp $(SERVERMANAGER) $(VIRTUAL_SERVER) $(PARSER) $(HTTP_PROTOCOL) $(CLIENT) $(TERMINAL) $(UTILS) $(Response)

HEADER_FILES = -Isrc/ServerManager/ServerCreator/ -Isrc/ServerManager/HTTP_Protocol \
				-Isrc/ServerManager/Utils -Isrc/ServerManager/VirtualServer/ -Isrc/ \
				-Isrc/Response/
FLAGS = -Wall -Wextra -Werror -std=c++98 $(HEADER_FILES) -DC98 -g
NAME = server
CC = c++

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm $(NAME)

re: clean all