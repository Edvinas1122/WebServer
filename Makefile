FORMATS = $(addprefix Formats/,configurationFileFormat.cpp httpFormat.cpp)
PARSER = $(addprefix Parsers/,AttributeGetter.cpp Parser.cpp DescendParser.cpp DefinitionExtractor.cpp mod/*.cpp $(FORMATS))
UTILS = $(addprefix src/Utils/,File.cpp $(PARSER) Terminal.cpp)
CLIENT = $(addprefix Client/,Client.cpp Tcp.cpp)
SERVER = $(addprefix src/Server/,Server.cpp PortSockets.cpp ConnectionQue.cpp Observer.cpp mod/*.cpp $(CLIENT))
VIRTUAL_SERVER = $(addprefix VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
REQUEST = $(addprefix Request/,HttpRequest.cpp)
RESPONSE = $(addprefix src/Response/,Error.cpp Response.cpp $(VIRTUAL_SERVER) $(REQUEST) mod/*.cpp)

SRC = src/main.cpp $(SERVER) $(RESPONSE) $(UTILS) $(TERMINAL)
TEST = src/test.cpp $(UTILS) $(RESPONSE)

HEADER_FILES = -Isrc/Utils/Parsers/ -Isrc/Utils/Parsers/Formats/ \
				-Isrc/Utils -Isrc/Response/VirtualServer/ -Isrc/ \
				-Isrc/Response/ -Isrc/Server/ -Isrc/Server/Client/
FLAGS = -Wall -Wextra -Werror -std=c++98 $(HEADER_FILES) -DC98 -DTERMINAL=1 -g
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