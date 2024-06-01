#pragma once
#include <ncurses.h>
#include "Board.hpp"
#include "Map.hpp"
#include "Snake.hpp"
#include "Game.hpp"

class View{
public:
    View() {
        initscr();
        noecho();
        curs_set(0);
    }

    ~View() {
        endwin();
    }

    void draw(Map map) {
        drawMap(map);   
        drawGame();
    }

    void drawMap(Map map) {
        game = newwin(22, 65, 6, 2);        // window to draw map
        refresh();
        
        for(int i = 0; i < map.mapY; i++) {     // draw map onto window
            for (int j = 0; j < map.mapX; j++){
                int number = map.getValue(i, j);
                if (number == 1 || number == 2) {
                    mvwprintw(game, i, j * 3, "o");
                }
                if (number == 3 || number == 4) {
                    mvwprintw(game, i, j * 3, "#");
                }
            }
        }
        wrefresh(game);     // Apply Changes at window screen
    }

    void drawGame() {
	    refresh();	
        
    	Game SnakeGame(22, 65, 200, game);    
        
	    while(!SnakeGame.over())
	    {
		    SnakeGame.input();

		    SnakeGame.update();

		    SnakeGame.reDraw();
	    }
    }

private:
    WINDOW *game;
    WINDOW *score;
};