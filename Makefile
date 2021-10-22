all:
	g++ -std=c++11 server.cpp logging.c -o server

clean:
	rm -f server