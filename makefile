# Compiler options
CXX	=	g++
CXXFLAGS	=	-pthread	-Wall	-g

# Executables
SERVER_EXEC	=	httpServer
CLIENT_EXEC	=	httpClient

# Source files
SERVER_SRC	=	server.cpp
CLIENT_SRC	=	client.cpp

# Images to Keep
#KEEP_IMAGES	=	img.jpg	testImage.png	# Add other images you want to keep here

all:	$(SERVER_EXEC)	$(CLIENT_EXEC)

$(SERVER_EXEC):	$(SERVER_SRC)
	$(CXX)	-o	$(SERVER_EXEC)	$(SERVER_SRC)	$(CXXFLAGS)

$(CLIENT_EXEC):	$(CLIENT_SRC)
	$(CXX)	-o	$(CLIENT_EXEC)	$(CLIENT_SRC)	$(CXXFLAGS)

clean:
	rm	-f	$(SERVER_EXEC)	$(CLIENT_EXEC)
	find	.	-name	"NewSaveimageData.*"	-type	f	-delete