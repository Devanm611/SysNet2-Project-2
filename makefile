all:
	g++	-o httpServer server.cpp -pthread -Wall -g
	g++	-o httpClient client.cpp -pthread -Wall -g
clean:
	rm -f httpServer httpClient