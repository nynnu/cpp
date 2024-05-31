#include "Map.hpp"
#include "View.hpp"
#include "Game.hpp"

int main() {
    initscr();
	refresh();	
	noecho();	

	curs_set(0);
	
	Game game(22, 65, 200);

	while(!game.over())
	{
		game.input();

		game.update();

		game.reDraw();
	}

	endwin();


    return 0;
}