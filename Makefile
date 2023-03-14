FORMATS = $(addprefix Formats/,configurationFileFormat.cpp)
PARSER = $(addprefix Parsers/,AttributeGetter.cpp Parser.cpp DescendParser.cpp DefinitionExtractor.cpp mod/*.cpp $(FORMATS))
UTILS =  $(addprefix src/Utils/,File.cpp $(PARSER))
UTILS_INCLUDE = -Isrc/Utils/ -Isrc/Utils/Parsers/ -Isrc/Utils/Parsers/Formats/
CLIENT = $(addprefix Client/,Client.cpp Tcp.cpp)
SERVER = $(addprefix src/Server/,Server.cpp PortSockets.cpp ConnectionQue.cpp Observer.cpp mod/*.cpp $(CLIENT))
VIRTUAL_SERVER = $(addprefix VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
REQUEST = $(addprefix src/Service/Request/,HttpRequest.cpp UrlQuery.cpp)
REQUEST_INCLUDE = -Isrc/Service/Request/
RESPONSE = $(addprefix src/Service/,Error.cpp Response.cpp $(VIRTUAL_SERVER) $(REQUEST) mod/*.cpp)
TERMINAL = $(addprefix src/Utils/,Terminal.cpp)

SRC = src/main.cpp $(SERVER) $(UTILS) $(TERMINAL)
TEST = src/test.cpp $(REQUEST) $(UTILS)

HEADER_FILES := -Isrc/
HEADER_FILES += $(if $(filter $(UTILS),$(SRC)), $(UTILS_INCLUDE))
HEADER_FILES += $(if $(filter $(REQUEST),$(SRC)), $(REQUEST_INCLUDE))
HEADER_FILES += -Isrc/Utils/Parsers/ -Isrc/Utils/Parsers/Formats/ \
				-Isrc/Service/VirtualServer/ -Isrc/ \
				-Isrc/Service/ -Isrc/Server/ -Isrc/Server/Client/
FLAGS = -Wall -Wextra -Werror -std=c++98 $(HEADER_FILES) -DC98 -DTERMINAL=1 -g
NAME = server
CC = c++

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

test: $(TEST)
	$(CC) $(FLAGS) $(TEST) -o service


clean:
	rm $(NAME)

re: clean all