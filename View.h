#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "Board.h"
#include "Map.h"
#include "Snake.h"
#include "Game.h"

class View {
public:
    View();
    ~View();

    void draw(int gateSpawnTime);
    void drawStartScreen();
    void drawGame(Map& map, int gateSpawnTime, bool& missionCompleted);
    void displayCongratulations();

private:
    WINDOW* gameWindow;
};

#endif // VIEW_H