#include <ncurses/ncurses.h>
#include <unistd.h>
#include <cstring>

const int SIZE = 3;
char board[SIZE][SIZE] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
char currentPlayer = 'X';
char player1Name[50], player2Name[50];

void Board(int startY, int startX) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            mvprintw(startY + i * 2, startX + j * 4, " %c ", board[i][j]);
            if (j < SIZE - 1) mvprintw(startY + i * 2, startX + j * 4 + 3, "|");
        }
        if (i < SIZE - 1) mvprintw(startY + i * 2 + 1, startX, "---+---+---");
    }
    refresh();
}

bool Win() {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true;
    
    return false;
}

bool Draw() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') return false;
        }
    }
    return true;
}

void Move(int choice) {
    int row = (choice - 1) / SIZE;
    int col = (choice - 1) % SIZE;

    if (board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

void PlayerName() {
    clear();
    
	int height = 7;
	int width = 50;
	int start_y = (LINES - height) / 2;
	int start_x = (COLS - width) / 2;
	
    WINDOW *menu_win = newwin(height, width, start_y, start_x);
	box(menu_win, 0, 0);
	mvwprintw(menu_win, 0, 1, "[ Enter Your Name ]");
	refresh();
	wrefresh(menu_win);
	
    mvprintw(13, 37, "Enter name for Player 1 (X): ");
    echo();
    getnstr(player1Name, 49);
    mvprintw(15, 37, "Enter name for Player 2 (O): ");
    getnstr(player2Name, 49);
    noecho();
}

void playTicTacToe() {
	
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    int startY = (maxY - (SIZE * 2 - 1)) / 2;
    int startX = (maxX - (SIZE * 4 - 1)) / 2;

    int choice;
    char previousPlayer = currentPlayer;
    while (true) {
        clear();
        Board(startY, startX);
        mvprintw(startY + SIZE * 2, startX - 10, "%s (X), enter your move (1-9): ", player1Name);
        if (currentPlayer == 'O') {
            mvprintw(startY + SIZE * 2, startX - 10, "%s (O), enter your move (1-9): ", player2Name);
        }
        refresh();

        choice = getch() - '0';
        if (choice >= 1 && choice <= 9) {
            Move(choice);

            if (Win()) {
                Board(startY, startX);
                if (previousPlayer == 'X') {
                    mvprintw(startY + SIZE * 2 + 2, startX + 1, "%s wins!", player1Name);
                } else {
                    mvprintw(startY + SIZE * 2 + 2, startX + 1, "%s wins!", player2Name);
                }
                break;
            }
            if (Draw()) {
                Board(startY, startX);
                mvprintw(startY + SIZE * 2 + 2, startX, "It's a draw!");
                break;
            }

            previousPlayer = currentPlayer;
        }
    }
    getch();
}

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
        mvprintw(y, x + strlen(loading_msg) + 1, "%c", animation[i % animation_length]);
        refresh();
        usleep(100000);
    }
    clear();
}

void Menu() {
    const char *choices[] = {"1. Start", "2. Exit"};
    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int highlight = 0;
    int c;

    int height = 6;
	int width = 20;
	int start_y = (LINES - height) / 2;
	int start_x = (COLS - width) / 2;
	
	WINDOW *menu_win = newwin(height, width, start_y, start_x);
	box(menu_win, 0, 0);
	mvwprintw(menu_win, 0, 1, "[ MENU OPTIONS ]");
	refresh();
	wrefresh(menu_win);

	while (1) {
		for (int i = 0; i < n_choices; i++) {
	            if (i == highlight) {
	                attron(A_REVERSE);
	                mvprintw(i + 14, 55, "%s", choices[i]);
	                attroff(A_REVERSE);
	            } else {
	                mvprintw(i + 14, 55, "%s", choices[i]);
	            }
	        }
	        refresh();

        c = getch();
        if (c == KEY_UP) {
            highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
        } else if (c == KEY_DOWN) {
            highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
        } else if (c == 10) {
            if (highlight == 0) {
                Loding();
                PlayerName();
                playTicTacToe();
            } else if (highlight == 1) {
                endwin();
            }
            break;
        }
    }
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    Menu();

    endwin();
    return 0;
}
