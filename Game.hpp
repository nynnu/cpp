#pragma once
#include <ncurses.h>
#include "Snake.hpp"
#include "Board.hpp"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "Apple.hpp"

class Game{
public:
    Game(int height, int width, int speed = 300) {
        board = Board(height, width, speed);
        initialize();
    }

    void initialize() {
        apple = NULL;
        board.initialize();
        gameOver = false;
        srand(time(NULL));      // 난수 생성기 시드 초기화
        
        snake.setDirection(downD);
        
        manage(SnakePiece(1,1));
        manage(snake.nextHead());
        manage(snake.nextHead());

        if (apple == NULL) {
            int x, y;
            apple = new Apple(10, 10);
        }
    }

    bool over() {
        return gameOver;
    }

    void input() {
        int oldTime = board.getTimeout();
        chtype input = board.getInput();
        switch (input)
        {
        case KEY_UP:
            snake.setDirection(upD);
            break;
        case KEY_DOWN:
            snake.setDirection(downD);
            break;
        case KEY_LEFT:
            snake.setDirection(leftD);
            break;
        case KEY_RIGHT:
            snake.setDirection(rightD);
            break;
        default:
            break;
        }
    }

    void update() {     // 계속 업데이트 해줄 메서드
        manage(snake.nextHead());
    }

    void reDraw()
	{
		board.refresh();
	}
private:
    Board board;
    Snake snake;
    int bodyCount = 3;      // 뱀의 길이 (길이가 3보다 작아지면 게임 종료)
    bool gameOver;
    Apple * apple;

    void manage(SnakePiece next) {
        if (apple != NULL) {
            switch (board.getChar(next.getY(), next.getX())) {
                case ' ': {
                    int emptyY = snake.tail().getY();       // 지울 곳의 y 좌표
	                int emptyX = snake.tail().getX();       // 지울 곳의 x 좌표
        	        board.addEmpty(SnakePiece(emptyY, emptyX));
                    snake.removePiece();
				    break;
                }
                case 'A':
                    break;
                default:
                    gameOver = true;
                    break;
            }
        }
        
        board.addAt(next.getY(), next.getX(), next.getI());
        snake.addPiece(next);
    }
};