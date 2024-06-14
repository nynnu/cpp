#pragma once
#include <ncurses.h>
#include "Snake.hpp"
#include "Board.hpp"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "Item.hpp"
#include "Map.hpp"

class Game {
public:
    Game(int height, int width, int speed, WINDOW* game, Map& map)
        : board(height, width, speed, game), itemManager(height, width), map(map) {
        initialize();
    }

    void initialize() {
        board.initialize();
        gameOver = false;
        srand(time(NULL)); // 난수 생성기 시드 초기화
        initializeBoardWithMap();
    }

    void initializeItems() {
        itemManager.spawnItems();
        itemManager.drawItems(board);
    }

    void initializeSnake(int startY, int startX) {
        snake.setDirection(downD);

        // 뱀 초기 위치 설정
        SnakePiece head(startY, startX, '@');
        snake.addPiece(head);
        board.add(head);
        for (int i = 1; i <= 2; ++i) {
            SnakePiece body(startY, startX - i, '#');
            snake.addPiece(body);
            board.add(body);
        }
    }

    bool over() const {
        return gameOver;
    }

    void input() {
        chtype input = board.getInput();
        switch (input) {
            case KEY_UP:
                if (snake.getDirection() != downD) snake.setDirection(upD);
                break;
            case KEY_DOWN:
                if (snake.getDirection() != upD) snake.setDirection(downD);
                break;
            case KEY_LEFT:
                if (snake.getDirection() != rightD) snake.setDirection(leftD);
                break;
            case KEY_RIGHT:
                if (snake.getDirection() != leftD) snake.setDirection(rightD);
                break;
            default:
                break;
        }
    }

    void update() {
        SnakePiece next = snake.nextHead();
        chtype ch = board.getChar(next.getY(), next.getX());
        if (ch == ' ' || ch == 'A') {
            next = SnakePiece(next.getY(), next.getX(), '@');
            addSnakePiece(next);
            if (ch == 'A') {
                itemManager.spawnItems();
            }
        } else if (ch == 'P') {
            if (snake.getLength() > 1) {
                SnakePiece tail = snake.tail();
                board.addEmpty(tail);
                snake.removePiece();
            } else {
                gameOver = true;
            }
        } else if (ch == 'o' || ch == '#' || ch == '@') {
            gameOver = true;
        }

        itemManager.drawItems(board);
    }

    void reDraw() {
        board.refresh();
    }

private:
    Board board;
    Snake snake;
    bool gameOver;
    ItemManager itemManager;
    Map& map;  // 맵 참조 추가

    void initializeBoardWithMap() {
        for (int y = 0; y < map.mapY; ++y) {
            for (int x = 0; x < map.mapX; ++x) {
                int value = map.getValue(y, x);
                if (value == 1 || value == 2) {
                    board.addAt(y, x, 'o');  // 벽 표시
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
