#pragma once
#include <ncurses.h>
class Board
{
public:
    Board(int height = 60, int width = 60)
    {
        int xMax, yMax;
        getmaxyx(stdscr, yMax, xMax);

        board_win = newwin(height, width * 3, 6, 2);
        initialize();
    }

    void initialize(){
        clear();
        refresh();
    }

    void addAt(int y, int x, chtype ch)
    {
        mvwaddch(board_win, y, x, ch);

    }

    chtype getInput()
    {
        return wgetch(board_win);
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
};