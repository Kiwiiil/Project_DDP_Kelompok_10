#include <ncurses/ncurses.h>
#include <cstring>
#include <unistd.h>

using namespace std;

void printMenu(WINDOW *menu_win, int highlight, const char *choices[], int n_choices) {
    int x = 2, y = 2;
    box(menu_win, 0, 0);
    for (int i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}


void loadingScreen() {
    const char *loading_msg = "Loading, please wait...";
    const char *animation = "|/-\\";
    int animation_length = 4;
    int x = (COLS - strlen(loading_msg)) / 2;
    int y = LINES / 2;

    for (int i = 0; i < 20; ++i) {
        clear();
        mvprintw(y - 1, x, "%s", loading_msg);
        mvprintw(y, x + strlen(loading_msg) + 1, "%c", animation[i % animation_length]);
        refresh();
        usleep(100000);
    }
    clear();
}

int main() {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    const char *choices[] = {"Start", "Exit"};
    int n_choices = sizeof(choices) / sizeof(choices[0]);

    int startx = 0, starty = 0;
    int width = 30, height = 10;
    int highlight = 1;
    int choice = 0;
    int c;

    loadingScreen();

    startx = (COLS - width) / 2;
    starty = (LINES - height) / 2;
    WINDOW *menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);

    while (true) {
        printMenu(menu_win, highlight, choices, n_choices);
        c = wgetch(menu_win);

        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }

        if (choice) {
            if (strcmp(choices[choice - 1], "Exit") == 0) {
                break;
            } else {
                mvprintw(LINES - 2, 0, "You chose: %s", choices[choice - 1]);
                refresh();
                getch();
            }
            choice = 0;
        }
    }

    clrtoeol();
    refresh();
    endwin();


    return 0;
}



   
