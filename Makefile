all: main

main: main.cpp *
	g++ -o main main.cpp Board.cpp Game.cpp Gate.cpp Item.cpp Snake.cpp Map.cpp View.cpp -lncurses