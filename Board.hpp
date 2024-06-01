#pragma once
#include <ncurses.h>
#include "Snake.hpp"
class Board
{
public:
    Board(int height = 60, int width = 60, int speed = 300)
    {
        timeout = speed;

        board_win = newwin(22, 65, 6, 2);
        wtimeout(board_win, timeout);
		keypad(board_win, true);
    }

    void initialize(){
        clear();
        refresh();
    }

    void add(SnakePiece Piece) {
        addAt(Piece.getY(), Piece.getX(), Piece.getI());
    }

    void addAt(int y, int x, chtype ch)
    {
        mvwaddch(board_win, y, x, ch);
    }

    void addEmpty(SnakePiece piece) {
        addAt(piece.getY(), piece.getX(), ' ');
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
        //wclear(board_win);
    }

    void refresh()
    {
        wrefresh(board_win);
    }

    WINDOW *board_win;
    int timeout;
};