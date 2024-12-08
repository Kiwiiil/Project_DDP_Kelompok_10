#include <ncurses/ncurses.h>
#include <windows.h>
#include <unistd.h>

using namespace std;

void Loding() {
const char *loading_msg = "Loading, please wait...";
    const char *animation = "|/-\\";
    int animation_length = 4;
    int x = (COLS - strlen(loading_msg)) / 2;
    int y = LINES / 2;

    for (int i = 0; i < 20; ++i) {
        clear();
        attron(A_BOLD | A_BLINK);
        mvprintw(y - 1, x, "%s", loading_msg);
        attroff(A_BOLD | A_BLINK);
        mvwprintw(stdscr, y, x + strlen(loading_msg) + 1, "%c", animation[i % animation_length]);
        refresh();
        usleep(100000);
    }
    clear();
}

int main() {
	initscr();
	
	Loding();

    refresh();
    endwin();

return 0;

}
