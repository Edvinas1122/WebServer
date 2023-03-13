FORMATS = $(addprefix Formats/,configurationFileFormat.cpp)
PARSER = $(addprefix Parsers/,AttributeGetter.cpp Parser.cpp DescendParser.cpp DefinitionExtractor.cpp mod/wordFind.cpp $(FORMATS))
UTILS = $(addprefix src/Utils/,File.cpp $(PARSER))
VIRTUAL_SERVER = $(addprefix src/ServerManager/VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
CLIENT = $(addprefix Client/,Client.cpp Tcp.cpp)
SERVER = $(addprefix src/Server/,Server.cpp PortSockets.cpp ConnectionQue.cpp Observer.cpp mod/openPortSocket.cpp $(CLIENT))
# HTTP_PROTOCOL = $(addprefix src/ServerManager/HTTP_Protocol/,HttpRequest.cpp HttpFormat.cpp MessageProcessor.cpp)
TERMINAL = $(addprefix src/ServerManager/,terminal.cpp)
RESPONSE = $(addprefix src/Response/,Error.cpp Response.cpp)
SRC = src/main.cpp $(SERVER) $(VIRTUAL_SERVER) $(TERMINAL) $(UTILS) $(RESPONSE)
TEST = src/test.cpp $(UTILS) $(HTTP_PROTOCOL)

HEADER_FILES = -Isrc/Utils/Parsers/ -Isrc/Utils/Parsers/Formats/ \
				-Isrc/Utils -Isrc/ServerManager/VirtualServer/ -Isrc/ \
				-Isrc/Response/ -Isrc/Server/
FLAGS = -Wall -Wextra -Werror -std=c++98 $(HEADER_FILES) -DC98 -g
NAME = server
CC = c++

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

test: $(TEST)
	$(CC) $(FLAGS) $(TEST) -o parser


clean:
	rm $(NAME)

re: clean all