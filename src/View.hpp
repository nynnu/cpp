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

        while (currentStage <= 4 && !missionCompleted) {
            try {
                Map stageMap(currentStage);
                gameWindow = newwin(stageMap.mapY, stageMap.mapX, 0, 0); 
                Game snakeGame = drawGame(stageMap, gateSpawnTime);

                if (snakeGame.missionAchieved()) {
                    currentStage++;
                    missionCompleted = (currentStage > 4);
                }

                delwin(gameWindow);

            } catch (const std::exception &e) {
                mvprintw(0, 0, "Error: %s", e.what());
                refresh();
                getch();
                break;
            }
        }

        if (missionCompleted) {
            displayCongratulations();
        }
    }

    Game drawGame(Map& map, int gateSpawnTime) {
        Game snakeGame(map.mapY, map.mapX, 200, gameWindow, map, gateSpawnTime);  

        while (!snakeGame.over()) {
            snakeGame.input();
            snakeGame.update();
            snakeGame.reDraw();

            if (snakeGame.missionAchieved()) {
                break;
            }
        }

        if (snakeGame.over() && !snakeGame.missionAchieved()) {
            snakeGame.displayGameOver();  // 게임 오버 메시지를 표시
        }

        return snakeGame;
    }

    void displayCongratulations() {
        mvwprintw(gameWindow, 10, 10, "CONGRATULATIONS! You've completed all stages!");
        wrefresh(gameWindow);
        nodelay(gameWindow, FALSE);  // 입력을 기다리도록 설정
        wgetch(gameWindow);  // 사용자 입력 대기
    }

private:
    WINDOW* gameWindow;
};
