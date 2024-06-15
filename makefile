all: main

main: main.cpp *
	g++ -o main main.cpp Map.cpp Game.cpp Snake.cpp Board.cpp View.cpp Item.cpp Gate.cpp -lncurses