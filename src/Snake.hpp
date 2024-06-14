//Snake.hpp

#pragma once
#include <ncurses.h>
#include <queue>
using namespace std;

enum DIRECTION {
    upD = -1, downD = 1, leftD = -2, rightD = 2
};

class SnakePiece {
public:
    SnakePiece(int y, int x, chtype ch = '#') : y(y), x(x), icon(ch) {}

    int getX() const { return x; }
    int getY() const { return y; }
    chtype getI() const { return icon; }

private:
    int x, y;
    chtype icon;
};

class Snake {
public:
    Snake() : curDirection(downD) {}

    void addPiece(SnakePiece piece) {
        prevPiece.push(piece);
    }

    void removePiece() {
        prevPiece.pop();
    }

    SnakePiece tail() const {
        return prevPiece.front();
    }

    SnakePiece head() const {
        return prevPiece.back();
    }

    int getLength() const {
        return prevPiece.size();
    }

    DIRECTION getDirection() const {
        return curDirection;
    }

    void setDirection(DIRECTION D) {
        curDirection = D;
    }

    SnakePiece nextHead() const {
        int row = head().getY();
        int col = head().getX();

        switch (curDirection) {
            case downD: row++; break;
            case upD: row--; break;
            case rightD: col++; break;
            case leftD: col--; break;
        }

        return SnakePiece(row, col, '@');
    }

private:
    queue<SnakePiece> prevPiece;
    DIRECTION curDirection;
};