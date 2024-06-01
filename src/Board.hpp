//Board.hpp

#pragma once
#include <ncurses.h>
#include "Snake.hpp"

class Board {
public:
    Board(int height, int width, int speed, WINDOW* game)
        : board_win(game), speed(speed) {
        initialize(); // 생성자에서 초기화를 진행할 수 있도록 initialize() 함수를 호출합니다.
    }

    void initialize() {
        wtimeout(board_win, speed); // 생성자에서 wtimeout을 호출하여 초기화합니다.
        keypad(board_win, true);
        clear();
        refresh();
    }

    void add(SnakePiece piece) {
        addAt(piece.getY(), piece.getX(), piece.getI());
    }

    void addAt(int y, int x, chtype ch) {
        mvwaddch(board_win, y, x, ch);
    }

    void addEmpty(SnakePiece piece) {
        addAt(piece.getY(), piece.getX(), ' ');
    }

    int getSpeed() const {
        return speed;
    }

    chtype getInput() const {
        return wgetch(board_win);
    }

    chtype getChar(int y, int x) const {
        return mvwinch(board_win, y, x);
    }

    void clear() {
        wclear(board_win);
    }

    void refresh() {
        wrefresh(board_win);
    }

private:
    WINDOW* board_win;
    int speed;
};
