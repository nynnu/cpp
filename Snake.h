#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>
#include <queue>

enum DIRECTION {
    upD = -1, downD = 1, leftD = -2, rightD = 2
};

class SnakePiece {
public:
    SnakePiece(int y, int x, chtype ch = '#') : y(y), x(x), icon(ch), direction(downD) {}

    int getX() const;
    int getY() const;
    chtype getI() const;
    DIRECTION getDirection() const;
    void setDirection(DIRECTION dir);

private:
    int x, y;
    chtype icon;
    DIRECTION direction;
};

class Snake {
public:
    Snake();

    void addPiece(SnakePiece piece);
    void removePiece();
    SnakePiece tail() const;
    SnakePiece head() const;
    int getLength() const;
    DIRECTION getDirection() const;
    bool canChangeDirection(DIRECTION newDirection) const;
    void setDirection(DIRECTION D);
    SnakePiece nextHead() const;

private:
    std::queue<SnakePiece> prevPiece;
    DIRECTION curDirection;
};

#endif // SNAKE_H
