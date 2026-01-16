#include <curses.h>
#include <stdio.h>

int main(void) {
    initscr();
    printw("Hello PDCurses!");
    refresh();
    getch();
    endwin();
    return 0;
}
