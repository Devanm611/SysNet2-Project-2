all:
	g++	-o	httpServer	server.cpp	-pthread	-Wall	-g
clean:
	rm	-f	httpServer