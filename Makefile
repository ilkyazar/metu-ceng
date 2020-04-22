all: 
	g++ -std=c++11 elevator.cpp -lpthread -o Elevator

clean:
	rm -f server