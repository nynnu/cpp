#pragma once
#include <ncurses.h>
class Board
{
public:
    Board(int height = 60, int width = 60, int speed = 300)
    {
        timeout = speed;

        board_win = newwin(height, width * 3, 6, 2);
        wtimeout(board_win, timeout);
		keypad(board_win, true);
    }

    void initialize(){
        clear();
        refresh();
    }

    void addAt(int y, int x, chtype ch)
    {
        mvwaddch(board_win, y, x, ch);
    }

    int getTimeout()
	{
		return timeout;
	}


    chtype getInput()
    {
        return wgetch(board_win);
    }

    chtype getChar(int y, int x)
	{
		return mvwinch(board_win, y, x);
	}

    void clear()  // 화면을 클리어
    {
        wclear(board_win);
    }

    void refresh()
    {
        wrefresh(board_win);
    }

    WINDOW *board_win;
    int timeout;
};