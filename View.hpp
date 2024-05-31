// show
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
        getch();
        endwin();
    }

    void draw(Map map, Snake snake) {
        drawMap(map);
    }

    void drawMap(Map map) {
        game = newwin(22, 65, 6, 2);
        refresh();
        
        for(int i = 0; i < map.mapY; i++) {
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
        wrefresh(game);
    }

private:
    WINDOW *game;
    WINDOW *score;
};