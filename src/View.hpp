// View.hpp

#pragma once
#include <ncurses.h>
#include "Board.hpp"
#include "Map.hpp"
#include "Snake.hpp"
#include "Game.hpp"

class View {
public:
    View() {
        initscr();
        noecho();
        curs_set(0);
    }

    ~View() {
        endwin();
    }

    void draw(Map& map, int gateSpawnTime) {
        gameWindow = newwin(map.mapY, map.mapX, 0, 0); 
        drawMap(map);
        drawGame(map, gateSpawnTime);
    }

    void drawMap(Map& map) {
        for (int i = 0; i < map.mapY; ++i) {
            for (int j = 0; j < map.mapX; ++j) {
                int number = map.getValue(i, j);
                if (number == 1) {  // 일반 벽 표시
                    mvwprintw(gameWindow, i, j, "o");
                } else if (number == 2) {  // immune wall 표시
                    mvwprintw(gameWindow, i, j, "O");
                } else if (number == 0) {  // 빈 공간 표시
                    mvwprintw(gameWindow, i, j, " ");
                } else if (number == 4) {  // 뱀 머리 표시
                    mvwprintw(gameWindow, i, j, "@");
                } else if (number == 3) {  // 뱀 몸통 표시
                    mvwprintw(gameWindow, i, j, "#");
                }
            }
        }
        wrefresh(gameWindow);  
    }

    void drawGame(Map& map, int gateSpawnTime) {
        Game snakeGame(map.mapY, map.mapX, 200, gameWindow, map, gateSpawnTime);  

        while (!snakeGame.over()) {
            snakeGame.input();
            snakeGame.update();
            snakeGame.reDraw();
        }

        // 게임 종료 메시지
        snakeGame.displayGameOver();  // 게임 오버 메시지를 표시
    }

private:
    WINDOW* gameWindow;
};
