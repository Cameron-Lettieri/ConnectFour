#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

#include "connect4.h"

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 6;
const char PLAYER_ONE_SYMBOL = 'O';
const char PLAYER_TWO_SYMBOL = 'X';

// Function to draw board
void drawBoard(std::vector < std::vector < char >> board) {
    clear();

    // Draw the top border of the board
    for (int i = 0; i < BOARD_WIDTH; i++) {
        attron(COLOR_PAIR(1));
        printw("+---");
        attroff(COLOR_PAIR(1));
    }
    attron(COLOR_PAIR(1));
    printw("+\n");
    attroff(COLOR_PAIR(1));

    // Draw the board
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            attron(COLOR_PAIR(1));
            printw("| ");
            attroff(COLOR_PAIR(1));

            // Print the symbol in the appropriate color
            if (board[i][j] == PLAYER_ONE_SYMBOL) {
                attron(COLOR_PAIR(2));
                printw("O ");
                attroff(COLOR_PAIR(2));
            } else if (board[i][j] == PLAYER_TWO_SYMBOL) {
                attron(COLOR_PAIR(3));
                printw("X ");
                attroff(COLOR_PAIR(3));
            } else {
                printw("  ");
            }
        }
        attron(COLOR_PAIR(1));
        printw("|\n");
        attroff(COLOR_PAIR(1));

        // Draw the bottom border of the row
        for (int j = 0; j < BOARD_WIDTH; j++) {
            attron(COLOR_PAIR(1));
            printw("+---");
            attroff(COLOR_PAIR(1));
        }
        attron(COLOR_PAIR(1));
        printw("+\n");
        attroff(COLOR_PAIR(1));
    }

    // Draw the column numbers at the bottom
    for (int i = 0; i < BOARD_WIDTH; i++) {
        attron(COLOR_PAIR(4));
        printw("  %d ", i + 1);
        attroff(COLOR_PAIR(4));
    }
    printw("\n\n");
}

// Function to check if a player has won
bool checkWin(std::vector < std::vector < char >> board, char symbol) {

    // Check horizontal wins
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH - 3; j++) {
            if (board[i][j] == symbol && board[i][j + 1] == symbol && board[i][j + 2] == symbol && board[i][j + 3] == symbol) {
                return true;
            }
        }
    }

    // Check vertical wins
    for (int i = 0; i < BOARD_HEIGHT - 3; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == symbol && board[i + 1][j] == symbol && board[i + 2][j] == symbol && board[i + 3][j] == symbol) {
                return true;
            }
        }
    }

    // Check diagonal wins (left to right)
    for (int i = 0; i < BOARD_HEIGHT - 3; i++) {
        for (int j = 0; j < BOARD_WIDTH - 3; j++) {
            if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol && board[i + 3][j + 3] == symbol) {
                return true;
            }
        }
    }

    // Check diagonal wins (right to left)
    for (int i = 0; i < BOARD_HEIGHT - 3; i++) {
        for (int j = 3; j < BOARD_WIDTH; j++) {
            if (board[i][j] == symbol && board[i + 1][j - 1] == symbol && board[i + 2][j - 2] == symbol && board[i + 3][j - 3] == symbol) {
                return true;
            }
        }
    }

    // No win
    return false;
}

void startMenu();

// Function for playing the  AI
void playAI() {
  
    std::vector < std::vector < char >> board(BOARD_HEIGHT, std::vector < char > (BOARD_WIDTH, ' '));

    char playerSymbol = PLAYER_ONE_SYMBOL;
    char aiSymbol = PLAYER_TWO_SYMBOL;

    char currentPlayer = PLAYER_ONE_SYMBOL;

    bool gameOver = false;
    while (!gameOver) {
        drawBoard(board);

        // Ask the current player for their move
        if (currentPlayer == playerSymbol) {
            attron(COLOR_PAIR(2));
        } else {
            attron(COLOR_PAIR(3));
        }
        printw("Player %c's turn\n", currentPlayer);
        attroff(COLOR_PAIR(2));
        attroff(COLOR_PAIR(3));
        printw("Enter a column (1-%d) to place your piece: ", BOARD_WIDTH);
        int column = 0;
        while (true) {
            int ch = getch();
            if (ch >= '1' && ch <= '7') {
                column = ch - '0' - 1;
                break;
            }
        }

        // Make the player's move
        int row = BOARD_HEIGHT - 1;
        while (row >= 0 && board[row][column] != ' ') {
            row--;
        }
        if (row >= 0) {
            board[row][column] = currentPlayer;
            if (checkWin(board, currentPlayer)) {
                gameOver = true;
            } else {
                // Switch to the other player
                if (currentPlayer == playerSymbol) {
                    currentPlayer = aiSymbol;
                } else {
                    currentPlayer = playerSymbol;
                }
            }
        }

        // Check if the game is over
        if (!gameOver) {
            // Make the AI's move
            int aiCol = rand() % BOARD_WIDTH;
            int aiRow = BOARD_HEIGHT - 1;
            while (aiRow >= 0 && board[aiRow][aiCol] != ' ') {
                aiRow--;
            }
            if (aiRow >= 0) {
                board[aiRow][aiCol] = aiSymbol;
                if (checkWin(board, aiSymbol)) {
                    gameOver = true;
                } else {
                    // Switch to the other player
                    currentPlayer = playerSymbol;
                }
            }
        }
    }

    drawBoard(board);

    // Print win or tie
    if (checkWin(board, playerSymbol)) {
        attron(COLOR_PAIR(2));
        printw("Player %c wins!\n", playerSymbol);
        attroff(COLOR_PAIR(2));
    } else if (checkWin(board, aiSymbol)) {
        attron(COLOR_PAIR(3));
        printw("AI wins!\n");
        attroff(COLOR_PAIR(3));
    } else {
        printw("Tie game!\n");
    }
    refresh();

    // User input to menu
    printw("Press any key to return to the menu...");
    getch();
    clear();
    startMenu();
}

// Function for playing another player
void playPlayer() {
 
    std::vector < std::vector < char >> board(BOARD_HEIGHT, std::vector < char > (BOARD_WIDTH, ' '));

    char currentPlayer = PLAYER_ONE_SYMBOL;

    bool gameOver = false;
    while (!gameOver) {
        drawBoard(board);

        // Ask the current player for their move
        if (currentPlayer == PLAYER_ONE_SYMBOL) {
            attron(COLOR_PAIR(2));
        } else {
            attron(COLOR_PAIR(3));
        }
        printw("Player %c's turn\n", currentPlayer);
        attroff(COLOR_PAIR(2));
        attroff(COLOR_PAIR(3));
        printw("Enter a column (1-%d) to place your piece: ", BOARD_WIDTH);
        int column = 0;
        while (true) {
            int ch = getch();
            if (ch >= '1' && ch <= '7') {
                column = ch - '0' - 1;
                break;
            }
        }

        // Make the move
        int row = BOARD_HEIGHT - 1;
        while (row >= 0 && board[row][column] != ' ') {
            row--;
        }
        if (row >= 0) {
            board[row][column] = currentPlayer;
            if (checkWin(board, currentPlayer)) {
                gameOver = true;
            } else {
                // Switch to the other player
                if (currentPlayer == PLAYER_ONE_SYMBOL) {
                    currentPlayer = PLAYER_TWO_SYMBOL;
                } else {
                    currentPlayer = PLAYER_ONE_SYMBOL;
                }
            }
        }

        // Check for a tie game
        bool tieGame = true;
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[i][j] == ' ') {
                    tieGame = false;
                }
            }
        }
        if (tieGame) {
            gameOver = true;
        }
    }

    drawBoard(board);

    // Print the win or tie
    if (checkWin(board, PLAYER_ONE_SYMBOL)) {
        attron(COLOR_PAIR(2));
        printw("Player %c wins!\n", PLAYER_ONE_SYMBOL);
        attroff(COLOR_PAIR(2));
    } else if (checkWin(board, PLAYER_TWO_SYMBOL)) {
        attron(COLOR_PAIR(3));
        printw("Player %c wins!\n", PLAYER_TWO_SYMBOL);
        attroff(COLOR_PAIR(3));
    } else {
        printw("Tie game!\n");
    }

    // User input to menu
    printw("Press any key to return to the menu...");
    getch();
    clear();
    startMenu();
}

void startMenu() {
    clear();
    printw("Welcome to Connect 4!\n\n");
    printw("Instructions:\n\n");
    printw("Use the the numbers 1-7 to drop your piece into the matching column.\n");
    printw("The first player to connect four pieces in a row wins the game!\n\n");
    printw("You can match 4 in a row in any direction:\n");
    printw("\t Vertical\n");
    printw("\t Horizontal\n");
    printw("\t Diagnol\n\n");
    printw("If the board fills up and no one matches 4, then the game ends in a tie!\n\n");
    printw("SELECT GAME MODE:\n");
    printw("1. Play vs AI\n");
    printw("2. Play vs another player\n\n");
    printw("press 'Q' to exit the game");
    refresh();

    int gameMode = 0;
    while (true) {
        int ch = getch();

        if (ch == '1') {
            gameMode = 1;
            break;
        } else if (ch == '2') {
            gameMode = 2;
            break;
        } else if (ch == 'Q') {
            break;
        }
    }

    // Call game mode
    if (gameMode == 1) {
        playAI();
    } else if (gameMode == 2) {
        playPlayer();
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);

    startMenu();

    endwin();

    return 0;
}
