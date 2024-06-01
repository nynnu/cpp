#pragma once
#include <ncurses.h>
#include <queue>
using namespace std;

enum DIRECTION {
    upD = -1, downD = 1, leftD = -2, rightD = 2
};

class SnakePiece {
public:
    SnakePiece(int y, int x, chtype ch = '#') {
        this->y = y;
        this->x = x;
        this->icon = ch;
    }
    
    int getX() {
        return x;
    }

    int getY(){
        return y;
    }

    chtype getI() {
        return icon;
    }
private:
    int x, y;
    chtype icon = ' ';
};

class Snake {
public:
    Snake() {
        curDirection = downD;
    }

    void addPiece(SnakePiece piece) {
        prevPiece.push(piece);
    }

    void removePiece() {
        prevPiece.pop();
    }

    SnakePiece tail() { 
        return prevPiece.front();       // 잘려나가는 부분 : 꼬리 = 빠지는 부분은 queue 의 앞부분
    }
    
    SnakePiece head() {
        return prevPiece.back();        // 꼬리와 동일한 방식
    }

    DIRECTION getDirection() {
        return curDirection;
    }

    void setDirection(DIRECTION D) {
        curDirection = D;
    }

    SnakePiece nextHead()       // 다음 머리를 불러올 함수
	{
		int row = head().getY();
		int col = head().getX();
		
        if(curDirection == downD) row++;
        else if(curDirection == upD) row--;
        else if(curDirection == rightD) col++;
        else if(curDirection == leftD) col--;

		return SnakePiece(row, col);        // 다음 헤드의 정보 전달
	}
private:
    queue<SnakePiece> prevPiece;   
    DIRECTION curDirection;     // 현재 방향

};