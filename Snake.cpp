#include "Snake.h"

// SnakePiece 클래스 멤버 함수 구현
int SnakePiece::getX() const {
    return x;
}

int SnakePiece::getY() const {
    return y;
}

chtype SnakePiece::getI() const {
    return icon;
}

DIRECTION SnakePiece::getDirection() const {
    return direction;
}

void SnakePiece::setDirection(DIRECTION dir) {
    direction = dir;
}

// Snake 클래스 멤버 함수 구현
Snake::Snake() : curDirection(downD) {}

void Snake::addPiece(SnakePiece piece) {
    piece.setDirection(curDirection);
    prevPiece.push(piece);
}

void Snake::removePiece() {
    prevPiece.pop();
}

SnakePiece Snake::tail() const {
    return prevPiece.front();
}

SnakePiece Snake::head() const {
    return prevPiece.back();
}

int Snake::getLength() const {
    return prevPiece.size();
}

DIRECTION Snake::getDirection() const {
    return curDirection;
}

bool Snake::canChangeDirection(DIRECTION newDirection) const {
    return curDirection != -newDirection;
}

void Snake::setDirection(DIRECTION D) {
    if (canChangeDirection(D)) {
        curDirection = D;
    }
}

SnakePiece Snake::nextHead() const {
    int row = head().getY();
    int col = head().getX();

    switch (curDirection) {
        case downD: row++; break;
        case upD: row--; break;
        case rightD: col++; break;
        case leftD: col--; break;
    }

    SnakePiece nextPiece(row, col, '@');
    nextPiece.setDirection(curDirection);
    return nextPiece;
}
