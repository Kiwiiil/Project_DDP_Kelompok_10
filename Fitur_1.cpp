#include <ncurses/ncurses.h>
#include <cstring>
#include <unistd.h>

using namespace std;

void printMenu(WINDOW *menu_win, int highlight, const char *choices[], int n_choices) {
    int x = 2, y = 2;

    wborder(menu_win, '|', '|', '=', '=', '+', '+', '+', '+');
    mvwprintw(menu_win, 0, 4, "[ MENU OPTIONS ]");

    for (int i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE | A_BOLD);
            mvwprintw(menu_win, y, x, "-> %s", choices[i]);
            wattroff(menu_win, A_REVERSE | A_BOLD);
        } else {
            mvwprintw(menu_win, y, x, "   %s", choices[i]);
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
        attron(A_BOLD | A_BLINK);
        mvprintw(y - 1, x, "%s", loading_msg);
        attroff(A_BOLD | A_BLINK);
        mvwprintw(stdscr, y, x + strlen(loading_msg) + 1, "%c", animation[i % animation_length]);
        refresh();
        usleep(100000);
    }
    clear();
}

void askUserName() {
    char name[50];
    mvprintw(LINES / 2, (COLS - 40) / 2, "Enter your name : ");
    echo();
    move(LINES / 2, (COLS - 50) / 2 + 23);
    getstr(name);
    noecho();
    attron(A_BOLD);
    mvprintw(LINES / 2 + 2, (COLS - 40) / 2, "Hello, %s! Welcome!", name);
    attroff(A_BOLD);
    refresh();
    getch();
}

void subMenu(WINDOW *menu_win) {
    const char *sub_choices[] = {"Back"};
    int n_sub_choices = sizeof(sub_choices) / sizeof(sub_choices[0]);
    int sub_highlight = 1;
    int sub_choice = 0;
    int c;
    int sub_startx = 0, sub_starty = 0;
    int sub_width = 40, sub_height = 8;

    sub_startx = (COLS - sub_width) / 2;
    sub_starty = (LINES - sub_height) / 2;
    WINDOW *sub_menu_win = newwin(sub_height, sub_width, sub_starty, sub_startx);
    keypad(sub_menu_win, TRUE);

    while (true) {
        wbkgd(sub_menu_win, COLOR_PAIR(1));
        printMenu(sub_menu_win, sub_highlight, sub_choices, n_sub_choices);
        c = wgetch(sub_menu_win);

        switch (c) {
            case KEY_UP:
                if (sub_highlight == 1)
                    sub_highlight = n_sub_choices;
                else
                    --sub_highlight;
                break;
            case KEY_DOWN:
                if (sub_highlight == n_sub_choices)
                    sub_highlight = 1;
                else
                    ++sub_highlight;
                break;
            case 10:
                sub_choice = sub_highlight;
                break;
            default:
                break;
        }

        if (sub_choice) {
            clear();
            if (strcmp(sub_choices[sub_choice - 1], "Back") == 0) {
                break;
            } else {
                attron(A_BOLD | COLOR_PAIR(2));
                mvprintw(LINES / 2, (COLS - 30) / 2, "You selected : %s", sub_choices[sub_choice - 1]);
                attroff(A_BOLD | COLOR_PAIR(2));
                refresh();
                getch();
            }
            sub_choice = 0;
            clear();
        }
    }
}

int main() {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
    }

    const char *choices[] = {"Start", "Exit"};
    int n_choices = sizeof(choices) / sizeof(choices[0]);

    int startx = 0, starty = 0;
    int width = 40, height = 8;
    int highlight = 1;
    int choice = 0;
    int c;

    loadingScreen();

    startx = (COLS - width) / 2;
    starty = (LINES - height) / 2;
    WINDOW *menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);

    wbkgd(menu_win, COLOR_PAIR(1));

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
            clear();
            if (strcmp(choices[choice - 1], "Exit") == 0) {
                mvprintw(LINES / 2, (COLS - 15) / 2, "Exiting program...");
                refresh();
                usleep(100000);
                break;
            } else {
                askUserName();
                subMenu(menu_win);
            }
            choice = 0;
            clear();
        }
    }

    clrtoeol();
    refresh();
    endwin();

    return 0;
}
