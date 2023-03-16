FORMATS = $(addprefix Formats/,configurationFileFormat.cpp)
PARSER = $(addprefix Parsers/,AttributeGetter.cpp Parser.cpp DescendParser.cpp DefinitionExtractor.cpp mod/*.cpp $(FORMATS))
UTILS =  $(addprefix src/Utils/,File.cpp $(PARSER))
UTILS_INCLUDE = -Isrc/Utils/ -Isrc/Utils/Parsers/ -Isrc/Utils/Parsers/Formats/

#Server
CLIENT = $(addprefix Client/,Client.cpp Tcp.cpp)
SERVER = $(addprefix src/Server/,Server.cpp PortSockets.cpp ConnectionQue.cpp Observer.cpp mod/*.cpp $(CLIENT))

#Services
REQUEST = $(addprefix Request/,HttpRequest.cpp UrlQuery.cpp)
REQUEST_INCLUDE = -Isrc/Services/Request/
VIRTUAL_SERVER = $(addprefix VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
WEBSITE = $(addprefix WebSite/,Error.cpp Response.cpp $(VIRTUAL_SERVER) $(REQUEST) mod/*.cpp)
TERMINAL = $(addprefix Terminal/,Terminal.cpp)
TERMINAL_INCLUDE = -Isrc/Services/Terminal/
SERVICE = $(addprefix src/Services/,Service.cpp $(WEBSITE) $(TERMINAL))

SRC = src/main.cpp $(SERVER) $(SERVICE) $(UTILS)
TEST = src/test.cpp $(SERVICE) $(UTILS)

HEADER_FILES := -Isrc/
HEADER_FILES += $(if $(filter $(UTILS),$(SRC)), $(UTILS_INCLUDE))
HEADER_FILES += $(if $(filter $(SERVICE),$(SRC)), $(REQUEST_INCLUDE))
HEADER_FILES += -Isrc/Utils/Parsers/ -Isrc/Utils/ -Isrc/Utils/Parsers/Formats/ -Isrc/Services/WebSite/ \
				-Isrc/Services/WebSite/VirtualServer/ -Isrc/ -Isrc/Services/WebSite/Request/ \
				-Isrc/Services/ -Isrc/Server/ -Isrc/Server/Client/ $(TERMINAL_INCLUDE)
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