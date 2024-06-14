//Item.hpp

#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "Snake.hpp"
#include "Board.hpp"

class Item {
public:
    Item(int y, int x, chtype icon) : y(y), x(x), icon(icon) {}

    int getY() const { return y; }
    int getX() const { return x; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    chtype getIcon() const { return icon; }

protected:
    int y, x;
    chtype icon;
};

class Apple : public Item {
public:
    Apple(int maxY, int maxX) : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'A') {}

    void spawn(int maxY, int maxX) {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
    }
};

class Poison : public Item {
public:
    Poison(int maxY, int maxX) : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'P') {}

    void spawn(int maxY, int maxX) {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
    }
};

class ItemManager {
public:
    ItemManager(int maxY, int maxX) : maxY(maxY), maxX(maxX) {
        srand(time(NULL)); // Seed for randomization
        apple = new Apple(maxY, maxX);
        poison = new Poison(maxY, maxX);
    }

    ~ItemManager() {
        delete apple;
        delete poison;
    }

    void spawnItems() {
        apple->spawn(maxY, maxX);
        poison->spawn(maxY, maxX);
    }

    bool snakeAteApple(const Snake& snake) const {
        return snake.head().getY() == apple->getY() && snake.head().getX() == apple->getX();
    }

    bool snakeAtePoison(const Snake& snake) const {
        return snake.head().getY() == poison->getY() && snake.head().getX() == poison->getX();
    }

    void drawItems(Board& board) const {
        board.addAt(apple->getY(), apple->getX(), apple->getIcon());
        board.addAt(poison->getY(), poison->getX(), poison->getIcon());
    }

private:
    Apple* apple;
    Poison* poison;
    int maxY, maxX;
};