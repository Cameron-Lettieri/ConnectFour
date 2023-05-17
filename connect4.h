#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <vector>

void drawBoard(std::vector<std::vector<char>> board);

bool checkWin(std::vector<std::vector<char>> board, char symbol);

void playAI();

void playPlayer();

void startMenu();


#endif
