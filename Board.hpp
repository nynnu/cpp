#pragma once
#include <ncurses.h>
#include <chrono>
#include "Snake.hpp"

class Board {
public:
    Board(int height, int width, int speed, WINDOW* game)
        : board_win(game), speed(speed), score_win(nullptr), mission_win(nullptr), start_time(std::chrono::high_resolution_clock::now()) {
        initialize();
    }

    ~Board() {
        if (score_win) delwin(score_win);
        if (mission_win) delwin(mission_win);
    }

    void initialize() {
        if (!score_win) score_win = newwin(11, 20, 0, 24);
        if (!mission_win) mission_win = newwin(11, 20, 11, 24);
        wtimeout(board_win, speed);
        keypad(board_win, true);
        clear();
        ScoreBoard(0, 0, 0, 0, 0);
        missionBoard('X', 'X', 'X', 'X', 0, 0, 0, 0);  // 초기 상태를 'X'로 설정
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

    void missionUpdate(char snakeSF, char appleSF, char poisonSF, char gateSF, int snakeM, int appleM, int poisonM, int gateM) {
        missionBoard(snakeSF, appleSF, poisonSF, gateSF, snakeM, appleM, poisonM, gateM);
    }

    WINDOW* getBoardWin() const {
        return board_win;
    }

private:
    void ScoreBoard(int snakeC, int maxSnakeC, int appleC, int poisonC, int gateC) {
        wmove(score_win, 0, 0);
        wborder(score_win, '|', '|', '-', '-', 'o', 'o', 'o', 'o');
        
        mvwprintw(score_win, 1, 4, "SCORE BOARD");
        mvwprintw(score_win, 3, 3, "B : %d / %d", snakeC, maxSnakeC);
        mvwprintw(score_win, 4, 3, "+ : %d", appleC);
        mvwprintw(score_win, 5, 3, "- : %d", poisonC);
        mvwprintw(score_win, 6, 3, "G : %d", gateC);

        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        mvwprintw(score_win, 7, 3, "T : %.2f", elapsed.count());

        wrefresh(score_win);
    }

    void missionBoard(char snakeSF, char appleSF, char poisonSF, char gateSF, int snakeM, int appleM, int poisonM, int gateM) {
        wmove(mission_win, 0, 0);
        wborder(mission_win, '|', '|', '-', '-', 'o', 'o', 'o', 'o');

        mvwprintw(mission_win, 1, 3, "MISSION BOARD");
        mvwprintw(mission_win, 3, 3, "B : %d (%c)", snakeM, snakeSF);
        mvwprintw(mission_win, 4, 3, "+ : %d (%c)", appleM, appleSF);
        mvwprintw(mission_win, 5, 3, "- : %d (%c)", poisonM, poisonSF);
        mvwprintw(mission_win, 6, 3, "G : %d (%c)", gateM, gateSF);

        wrefresh(mission_win);
    }

    WINDOW* board_win;
    WINDOW* score_win;
    WINDOW* mission_win;
    int speed;
    int snakeC{0}, maxSnakeC{0}, appleC{0}, poisonC{0}, gateC{0};
    int snakeM{0}, appleM{0}, poisonM{0}, gateM{0};
    char snakeSF{'X'}, appleSF{'X'}, poisonSF{'X'}, gateSF{'X'};

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
