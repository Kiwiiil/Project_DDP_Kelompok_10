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

void Menu() {
	const char *choices[] = {
        "1. Start",
        "2. Exit"
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int highlight = 0;
    int c;
    box(stdscr, 0, 0);

    while (1) {
    
        for (int i = 0; i < n_choices; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
                mvprintw(i + 14, 56, "%s", choices[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(i + 14, 56, "%s", choices[i]);
            }
        }
        refresh();
        
        c = getch();
        if (c == KEY_UP) {
            highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
        } else if (c == KEY_DOWN) {
            highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
        } else if (c == 10) {
            break;
        }
    }
    clear();
}

int main() {
	initscr();
	clear();
    noecho();
    cbreak();
	
	Loding();
	Menu();

    refresh();
    endwin();

return 0;

}
