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
    SnakePiece(int y, int x, chtype ch = '#') : y(y), x(x), icon(ch), direction(downD) {}

    int getX() const { return x; }
    int getY() const { return y; }
    chtype getI() const { return icon; }
    DIRECTION getDirection() const { return direction; } // 새로운 메서드 추가

    void setDirection(DIRECTION dir) { direction = dir; } // 방향 설정 메서드 추가

private:
    int x, y;
    chtype icon;
    DIRECTION direction; // 방향 필드 추가
};

class Snake {
public:
    Snake() : curDirection(downD) {}

    void addPiece(SnakePiece piece) {
        piece.setDirection(curDirection); // 조각의 방향 설정
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

    bool canChangeDirection(DIRECTION newDirection) const {
        return curDirection != -newDirection;
    }

    void setDirection(DIRECTION D) {
        if (canChangeDirection(D)) {
            curDirection = D;
        }
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

        SnakePiece nextPiece(row, col, '@');
        nextPiece.setDirection(curDirection); // 다음 조각의 방향 설정
        return nextPiece;
    }

private:
    queue<SnakePiece> prevPiece;
    DIRECTION curDirection;
};
