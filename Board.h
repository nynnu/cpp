#ifndef BOARD_HPP
#define BOARD_HPP

#include <ncurses.h>
#include <chrono>
#include "Snake.h" // Assuming Snake.hpp exists and is correctly defined

class Board {
public:
    Board(int height, int width, int speed, WINDOW* game);
    ~Board();

    void initialize();
    void add(SnakePiece piece);
    void addAt(int y, int x, chtype ch);
    void addEmpty(SnakePiece piece);
    int getSpeed() const;
    void setSpeed(int newSpeed);
    chtype getInput() const;
    chtype getChar(int y, int x) const;
    void clear();
    void refresh();
    void scoreUpdate(int snakeC, int maxSnakeC, int appleC, int poisonC, int gateC);
    void missionUpdate(char snakeSF, char appleSF, char poisonSF, char gateSF, int snakeM, int appleM, int poisonM, int gateM);
    WINDOW* getBoardWin() const;

private:
    void ScoreBoard(int snakeC, int maxSnakeC, int appleC, int poisonC, int gateC);
    void missionBoard(char snakeSF, char appleSF, char poisonSF, char gateSF, int snakeM, int appleM, int poisonM, int gateM);

    WINDOW* board_win;
    WINDOW* score_win;
    WINDOW* mission_win;
    int speed;
    int snakeC{0}, maxSnakeC{0}, appleC{0}, poisonC{0}, gateC{0};
    int snakeM{0}, appleM{0}, poisonM{0}, gateM{0};
    char snakeSF{'X'}, appleSF{'X'}, poisonSF{'X'}, gateSF{'X'};

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

#endif // BOARD_HPP
