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
        start_color();
    }

    ~View() {
        endwin();
    }

    void draw(int gateSpawnTime) {
        int currentStage = 1;
        bool missionCompleted = false;

        while (currentStage <= 4) {
            try {
                Map stageMap(currentStage);
                gameWindow = newwin(stageMap.mapY, stageMap.mapX, 0, 0); 
                drawGame(stageMap, gateSpawnTime, missionCompleted);
                delwin(gameWindow);

                if (missionCompleted) {
                    currentStage++;
                    if (currentStage > 4) {
                        displayCongratulations();
                        break;
                    }
                }
            } catch (const std::exception &e) {
                mvprintw(0, 0, "Error: %s", e.what());
                refresh();
                getch();
                break;
            }
        }
    }

    void drawGame(Map& map, int gateSpawnTime, bool& missionCompleted) {
        Game snakeGame(map.mapY, map.mapX, 200, gameWindow, map, gateSpawnTime);  

        while (!snakeGame.over()) {
            snakeGame.input();
            snakeGame.update();
            snakeGame.reDraw();

            if (snakeGame.missionAchieved()) {
                missionCompleted = true;
                break;
            }
        }

        if (snakeGame.over() && !snakeGame.missionAchieved()) {
            snakeGame.displayGameOver();  // 게임 오버 메시지를 표시
        }
    }

    void displayCongratulations() {
        clear();
        mvprintw(LINES / 2, (COLS - 30) / 2, "CONGRATULATIONS! You've completed all stages!");
        mvprintw(LINES / 2 + 1, (COLS - 30) / 2, "Press any key to exit...");
        refresh();
        nodelay(stdscr, FALSE);  // 입력을 기다리도록 설정
        getch();  // 사용자 입력 대기
    }

private:
    WINDOW* gameWindow;
};
