all: main

main: main.cpp *
	g++ -o main main.cpp Map.cpp -lncurses