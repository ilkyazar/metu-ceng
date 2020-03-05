all:
	g++ -std=c++11 server.cpp ./lib/logging.c -o server

clean:
	rm -f server