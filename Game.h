#ifndef GAME_HPP
#define GAME_HPP

#include <ncurses.h>
#include "Snake.h"
#include "Board.h"
#include "Gate.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Item.h"
#include "Map.h"
#include <random>

class Game {
public:
    Game(int height, int width, int speed, WINDOW* game, Map& map, int gateSpawnTime);
    void initialize();
    void initializeSnake(int startY, int startX);
    bool over() const;
    bool missionAchieved() const;
    void randomMission();
    void missionComplete();
    void input();
    void update();
    void reDraw();
    void displayGameOver();
    void checkAndUpdateSpeed();

private:
    Board board;
    Snake snake;
    bool gameOver;
    ItemManager itemManager;
    Gate gate;
    Map& map;
    int snakeC{3}, maxSnakeC{10}, appleC{0}, poisonC{0}, gateC{0};
    char snakeSF{'O'}, appleSF{'X'}, poisonSF{'X'}, gateSF{'X'};
    int snakeM{0}, appleM{0}, poisonM{0}, gateM{0};

    void initializeBoardWithMap();
    void addSnakePiece(SnakePiece next);
};

#endif // GAME_HPP
