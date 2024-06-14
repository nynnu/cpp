//View.hpp


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

    void draw(Map& map) {
        gameWindow = newwin(map.mapY, map.mapX, 0, 0);  // window to draw map
        drawMap(map);
        drawGame(map);
    }

    void drawMap(Map& map) {
        for (int i = 0; i < map.mapY; ++i) {
            for (int j = 0; j < map.mapX; ++j) {
                int number = map.getValue(i, j);
                if (number == 1 || number == 2) {  // 벽 표시
                    mvwprintw(gameWindow, i, j, "o");
                } else if (number == 0) {  // 빈 공간 표시
                    mvwprintw(gameWindow, i, j, " ");
                } else if (number == 4) {  // 뱀 머리 표시
                    mvwprintw(gameWindow, i, j, "@");
                } else if (number == 3) {  // 뱀 몸통 표시
                    mvwprintw(gameWindow, i, j, "#");
                }
            }
        }
        wrefresh(gameWindow);  // Apply Changes at window screen
    }

    void drawGame(Map& map) {
        Game snakeGame(map.mapY, map.mapX, 200, gameWindow, map);  // map 객체를 전달

        // 뱀 초기 위치 설정
        std::vector<int> headPos = map.getPos(4);
        if (!headPos.empty()) {
            snakeGame.initializeSnake(headPos[0], headPos[1]);
        }

        // 아이템 초기 위치 설정
        snakeGame.initializeItems();

        while (!snakeGame.over()) {
            snakeGame.input();
            snakeGame.update();
            snakeGame.reDraw();
        }

        // 게임 종료 메시지
        mvprintw(map.mapY + 2, 0, "Game Over! Press any key to exit.");
        refresh();
        getch();
    }

private:
    WINDOW* gameWindow;
};
