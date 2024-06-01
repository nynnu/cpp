#pragma once
#include <ncurses.h>

class Apple {
public:
	Apple(int y, int x)
	{
		this->y = y;
		this->x = x;
		this->icon = 'A';
	}
private:
    int x, y;
    chtype icon;
};