// Game.hpp
#pragma once
#include <ncurses.h>
#include "Snake.hpp"
#include "Board.hpp"
#include "Gate.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Item.hpp"
#include "Map.hpp"
#include <random>

// Game 클래스는 게임의 전반적인 로직과 상태 관리
class Game {
public:
    // 생성자: 게임 보드, 뱀, 아이템 관리자, 게이트, 맵 초기화
    Game(int height, int width, int speed, WINDOW* game, Map& map, int gateSpawnTime)
        : board(height, width, speed, game), snake(), itemManager(height, width), gate(height, width, gateSpawnTime), map(map) {
        initialize();
    }

    // 게임 초기화 함수
    void initialize() {
        board.initialize();
        gameOver = false;
        srand(time(NULL)); 
        initializeBoardWithMap();
        gate.initialize(map, board);
        randomMission();
    }

    // 뱀 초기화 함수
    void initializeSnake(int startY, int startX) {
        snake.setDirection(downD);
    }

    // 게임 오버 상태 확인 함수
    bool over() const {
        return gameOver;
    }

    bool missionAchieved() const {
        return snakeSF == 'O' && appleSF == 'O' && poisonSF == 'O' && gateSF == 'O';
    }

    void randomMission() {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_int_distribution<> dist(1, 2);

        snakeM = dist(gen);
        appleM = dist(gen);
        poisonM = dist(gen);
        gateM = dist(gen);
    }

    void missionComplete() {
        if(appleC >= appleM) appleSF = 'O';
        if(poisonC >= poisonM) poisonSF = 'O';
        if(gateC >= gateM) gateSF = 'O';
    }

    // 사용자 입력 처리 함수.
    void input() {
        chtype input = board.getInput();
        switch (input) {
            case KEY_UP:
                if (!snake.canChangeDirection(upD)) {
                    gameOver = true; // 반대 방향으로 이동하려 할 때 게임 오버
                }
                if (snake.getDirection() != downD) snake.setDirection(upD);
                break;
            case KEY_DOWN:
                if (!snake.canChangeDirection(downD)) {
                    gameOver = true; // 반대 방향으로 이동하려 할 때 게임 오버
                }
                if (snake.getDirection() != upD) snake.setDirection(downD);
                break;
            case KEY_LEFT:
                if (!snake.canChangeDirection(leftD)) {
                    gameOver = true; // 반대 방향으로 이동하려 할 때 게임 오버
                }
                if (snake.getDirection() != rightD) snake.setDirection(leftD);
                break;
            case KEY_RIGHT:
                if (!snake.canChangeDirection(rightD)) {
                    gameOver = true; // 반대 방향으로 이동하려 할 때 게임 오버
                }
                if (snake.getDirection() != leftD) snake.setDirection(rightD);
                break;
            default:
                break;
        }
    }

    // 게임 상태 갱신 함수
    void update() {
        SnakePiece next = snake.nextHead();  // 뱀의 다음 머리 위치 계산
        chtype ch = board.getChar(next.getY(), next.getX());

        gate.update(board); // 게이트 상태 갱신

        // 뱀이 자신의 몸통과 충돌하는지 확인
        if (ch == '#') {
            gameOver = true;
        } else {
            // 뱀이 빈 공간이나 사과를 먹은 경우
            if (ch == ' ' || ch == 'A') {
                next = SnakePiece(next.getY(), next.getX(), '@');
                addSnakePiece(next);
                if (ch == 'A') {
                    snake.addPiece(next);  // 뱀 몸통 추가
                    itemManager.removeItemAt(next.getY(), next.getX());  // 아이템 제거
                    ++snakeC;
                    ++appleC;
                }
            // 뱀이 독을 먹은 경우
            } else if (ch == 'P') {
                if (snake.getLength() > 1) {
                    SnakePiece tail = snake.tail();
                    board.addEmpty(tail);
                    snake.removePiece();
                    itemManager.removeItemAt(next.getY(), next.getX());  // 아이템 제거
                    --snakeC;
                    ++poisonC;
                    board.addEmpty(next);  // 아이템 제거 후 해당 위치를 지우기 위해 추가
                    if (snake.getLength() < 3) {
                        gameOver = true;  // 몸 길이가 3보다 작아지면 게임 오버
                    }
                } else {
                    gameOver = true;
                }
            // 뱀이 벽이나 자기 자신과 충돌한 경우
            } else if (ch == 'o' || ch == '@') {
                gameOver = true;
            } else if (ch == 'G') {
                DIRECTION newDirection;
                auto exitPos = gate.getNextPosition(next, newDirection);
                if (exitPos.first != -1 && exitPos.second != -1) {
                    snake.setDirection(newDirection);
                    next = SnakePiece(exitPos.first, exitPos.second, '@');
                    addSnakePiece(next);
                    ++gateC;
                }
            }

            board.scoreUpdate(snakeC, maxSnakeC, appleC, poisonC, gateC);
            missionComplete();
            board.missionUpdate(snakeSF, appleSF, poisonSF, gateSF, snakeM, appleM, poisonM, gateM);
            itemManager.updateItems(board);  // 아이템을 주기적으로 갱신
            itemManager.drawItems(board);

            // 게임 오버 상태라면 게임 오버 메시지 표시
            if (gameOver) {
                displayGameOver();
            }
        }
    }

    // 게임 화면 갱신 함수
    void reDraw() {
        board.refresh();
    }

    // 게임 오버 메시지 표시 함수
    void displayGameOver() {
        WINDOW* win = board.getBoardWin();
        mvwprintw(win, map.mapY / 2, (map.mapX / 2) - 5, "GAME OVER!");
        mvwprintw(win, (map.mapY / 2) + 1, (map.mapX / 2) - 10, "Press any key to exit...");
        wrefresh(win);
        nodelay(win, FALSE);  // 입력을 기다리도록 설정
        wgetch(win);  // 사용자 입력 대기
    }

private:
    Board board;  
    Snake snake;  
    bool gameOver;  // 게임 오버 상태
    ItemManager itemManager;  
    Gate gate;
    Map& map;  
    int snakeC{3}, maxSnakeC{10}, appleC{0}, poisonC{0}, gateC{0};
    char snakeSF{'O'}, appleSF{'X'}, poisonSF{'X'}, gateSF{'X'};
    int snakeM{0}, appleM{0}, poisonM{0}, gateM{0};

    // 맵을 초기화하고 뱀을 맵에 배치하는 함수
    void initializeBoardWithMap() {
        for (int y = 0; y < map.mapY; ++y) {
            for (int x = 0; x < map.mapX; ++x) {
                int value = map.getValue(y, x);
                if (value == 1) {
                    board.addAt(y, x, 'o');
                } else if (value == 2) {
                    board.addAt(y, x, 'O'); // Immune wall 표시
                } else if (value == 3) {
                    SnakePiece body(y, x, '#');
                    snake.addPiece(body);
                    board.add(body);
                } else if (value == 4) {
                    SnakePiece head(y, x, '@');
                    snake.addPiece(head);
                    board.add(head);
                }
            }
        }
    }

    // 뱀의 새로운 머리를 추가하고 꼬리를 제거하는 함수
    void addSnakePiece(SnakePiece next) {
        if (snake.getLength() > 0) {
            SnakePiece head = snake.head();
            board.add(SnakePiece(head.getY(), head.getX(), '#'));
        }
        board.add(next);
        snake.addPiece(next);
        if (snake.getLength() > 3) {
            SnakePiece tail = snake.tail();
            board.addEmpty(tail);
            snake.removePiece();
        }
    }
};