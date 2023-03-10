PARSER = $(addprefix src/ServerManager/ServerCreator/,DescendParser.cpp AttributeGetter.cpp DefinitionExtractor.cpp ConfigurationFile.cpp wordFind.cpp openPortSocket.cpp)
VIRTUAL_SERVER = $(addprefix src/ServerManager/VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
SERVERMANAGER = $(addprefix src/ServerManager/,ServerManager.cpp)
SRC = src/main.cpp $(SERVERMANAGER) $(VIRTUAL_SERVER) $(PARSER)

HEADER_FILES = -Isrc/ServerManager/ServerCreator/ -Isrc/ 
FLAGS = -Wall -Wextra -Werror -std=c++98 $(HEADER_FILES) -DC98
NAME = server
CC = c++

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm $(NAME)

re: clean all