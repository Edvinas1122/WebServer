FORMATS = $(addprefix Formats/,configurationFileFormat.cpp)
PARSER = $(addprefix Parsers/,AttributeGetter.cpp Parser.cpp DescendParser.cpp DefinitionExtractor.cpp mod/*.cpp $(FORMATS))
UTILS =  $(addprefix src/Utils/,File.cpp Executor.cpp $(PARSER))
UTILS_INCLUDE = -Isrc/Utils/ -Isrc/Utils/Parsers/ -Isrc/Utils/Parsers/Formats/

#Server
CONNECTION = $(addprefix Connection/,Timer.cpp Connection.cpp TCP.cpp)
SERVER = $(addprefix src/Server/,Server.cpp PortSockets.cpp ConnectionQue.cpp Observer.cpp mod/*.cpp $(CONNECTION))

#Services
REQUEST = $(addprefix Request/,HttpRequest.cpp UrlQuery.cpp dic/*.cpp)
# REQUEST_INCLUDE = -Isrc/Services/Request/
VIRTUAL_SERVER = $(addprefix VirtualServer/,VirtualServer.cpp Route.cpp VirtualServerValidators.cpp)
# RESPONSE = $(addprefix Response/,Error.cpp Response.cpp)
# UPLOAD = $(addprefix Upload/,UploadService.cpp mod/*.cpp)
# WEBSITE = $(addprefix WebSite/,WebSite.cpp Download.cpp FileUpload.cpp $(REQUEST) $(VIRTUAL_SERVER) mod/*.cpp)
WEBSITE = $(addprefix WebSite/,WebSite.cpp HTTPParser.cpp Transmision.cpp HTTPCodes.cpp $(REQUEST) $(VIRTUAL_SERVER) mod/*.cpp)
TELNET_SERVICE = $(addprefix TelNetServer/, TelNetServer.cpp)
PROCESSES = $(addprefix Processes/,Processes.cpp ExecuteFile.cpp)
SERVICE = $(addprefix src/Services/,Service.cpp ServiceProcess.cpp TimeOut.cpp $(WEBSITE) $(TELNET_SERVICE) $(PROCESSES))
INTERFACE = $(addprefix src/Interface/, ProgramInterface.cpp)

SRC = src/main.cpp $(SERVER) $(SERVICE) $(UTILS) $(INTERFACE)
TEST = src/test.cpp $(SERVER) $(SERVICE) $(UTILS)

HEADER_FILES := -Isrc/
HEADER_FILES += $(if $(filter $(UTILS),$(SRC)), $(UTILS_INCLUDE))
HEADER_FILES += $(if $(filter $(SERVICE),$(SRC)), $(REQUEST_INCLUDE))
HEADER_FILES += -Isrc/Utils/Parsers/ -Isrc/Utils/ -Isrc/Utils/Parsers/Formats/\
			 -Isrc/ -Isrc/Services/WebSite/Request/ -Isrc/Services/Processes/ -Isrc/Services/TelNetServer/\
				-Isrc/Services/ -Isrc/Services/WebSite/VirtualServer -Isrc/Server/\
				-Isrc/Server/Connection/ -Isrc/Interface/ -Isrc/Services/WebSite $(TERMINAL_INCLUDE)
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