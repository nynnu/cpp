// Board.hpp
#pragma once
#include <ncurses.h>
#include "Snake.hpp"

class Board {
public:
    Board(int height, int width, int speed, WINDOW* game)
        : board_win(game), speed(speed) {
        initialize();
    }

    void initialize() {
        wtimeout(board_win, speed);
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

    WINDOW* getBoardWin() const {  // board_win을 접근할 수 있는 메서드 추가
        return board_win;
    }

private:
    WINDOW* board_win;
    int speed;
};
