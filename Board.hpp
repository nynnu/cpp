
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
        ScoreBoard(snakeC, maxSnakeC,0,0,0);
        missionBoard();
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

    void scoreUpdate(int snakeC, int maxSnakeC, int appleC, int poisonC, int gateC) {
        ScoreBoard(snakeC, maxSnakeC, appleC, poisonC, gateC);  // scoreBoard update
    }

    WINDOW* getBoardWin() const {
        return board_win;
    }

    void ScoreBoard(int snakeC, int maxSnakeC, int appleC, int poisonC, int gateC) {
        wmove(score_win, 0, 0);
        wborder(score_win, '|','|','-','-','o','o','o','o');
        
        mvwprintw(score_win, 1, 4, "SCORE BOARD");
        mvwprintw(score_win, 3, 3, "B : %d / %d", snakeC, maxSnakeC);
        mvwprintw(score_win, 4, 3, "+ : %d", appleC);
        mvwprintw(score_win, 5, 3, "- : %d", poisonC);
        mvwprintw(score_win, 6, 3, "G : %d ", 0);

        wrefresh(score_win);
    }

    void missionBoard() {
        wmove(mission_win, 0, 0);
        wborder(mission_win, '|','|','-','-','o','o','o','o');

        mvwprintw(mission_win, 1, 3, "MISSION BOARD");

        wrefresh(mission_win);
    }

private:
    WINDOW* board_win;
    WINDOW *score_win = newwin(11, 20, 0, 24);
    WINDOW *mission_win = newwin(11, 20, 11, 24);
    int speed, snakeC{0}, maxSnakeC{0}, appleC{0}, poisonC{0}, gateC{0};
};
