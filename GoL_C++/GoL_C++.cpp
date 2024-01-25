// GoL_C++.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <windows.h>
#include <stdio.h>
const int width = 210;
const int height = 126;
SHORT fontSize = 1;
int chanceOfInitialCell = 10;
bool cells[height][width];
bool cellsUpdated[height][width];
const int maxListSize = 10;
int timesBoardsCanMatch = 50;
char lastboards[maxListSize][width * height + height];
char board[width * height + height];
int main()
{
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, new CONSOLE_FONT_INFOEX{ sizeof(CONSOLE_FONT_INFOEX), 0, { fontSize,fontSize } });
    do
    {
        bool gameOver = false;
        int amountInArray = 0, timesBoardsMatched = 0;
        for (unsigned short i = 0; i < width * height; i++)
            cells[i % height][i / height] = rand() % 100 <= chanceOfInitialCell;
        while (!gameOver) {
            Sleep(50);
            int boardIndex = 0;
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                {
                    int neighborsAlive = 0;
                    for (int ny = (y - 1 + height) % height; ny != (y + 2 + height) % height; ny = (ny + 1) % height)
                        for (int nx = (x - 1 + width) % width; nx != (x + 2 + width) % width; nx = (nx + 1) % width)
                            if (!(x == nx && y == ny)) neighborsAlive += cells[ny][nx];
                    cellsUpdated[y][x] = neighborsAlive == 3 || (cells[y][x] && neighborsAlive == 2);
                    board[boardIndex++] = cellsUpdated[y][x] ? 219 : ' ';//You can replace 219 with 'O'
                    if (x + 1 == width)
                        board[boardIndex++] = '\n';
                }
            printf("\33[?25l\33[8;%i;%it\33[;H%s", (height + 1), (width), board);//Turn Cursor Invisible and Set height and width, Set Cursor Position to Top and Print board
            bool matchFound = false;
            for (size_t i = 0; i < maxListSize; i++)
                if (memcmp(lastboards[i], board, sizeof(board)) == 0)
                    matchFound = true;
            gameOver = matchFound ? timesBoardsMatched++ : 0 >= timesBoardsCanMatch;
            memcpy(lastboards[(amountInArray = amountInArray++ % maxListSize)], board, sizeof(board));
            memcpy(cells, cellsUpdated, sizeof(cells));
        }
    } while (getchar() == '\n');
}
