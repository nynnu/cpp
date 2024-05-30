#include "Map.h"
#include <ncurses.h>
#include "Board.hpp"

int main(int argc, char **argv)
{
    initscr();
    refresh();

    Map map(1);

    refresh();

    getch();
    endwin();

    return 0;
}