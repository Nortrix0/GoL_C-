// GoL_C++.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <windows.h>
#include <random>
#include <iostream>
#include <bitset>
const int width = 118;//To have a single char represent a cell multiply this by 2
const int height = 62;
const int chanceOfInitialCell = 10;
bool cells[height][width];
char board[width*2 * height + height];//To have a single char represent a cell remove the multiply by 2
bool gameOver = false;
const int maxListSize = 10;
int timesBoardsMatched = 0;
int timesBoardsCanMatch = 50;
char lastboards[maxListSize][width*2 * height + height];//To have a single char represent a cell remove the multiply by 2
int amountInArray = 0;
const HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
const COORD dest = {0,0};
const CONSOLE_CURSOR_INFO cursorinfo = {1, 0};
void generateInitialCells() {
    std::random_device rand;
    for (size_t i = 0; i < width * height; i++)
            cells[i % height][i / height] = rand() % 100 <= chanceOfInitialCell;
}
void updateAndRedrawCells() {
    int boardIndex = 0;
    bool cellsUpdated[height][width];
    memcpy(cellsUpdated, cells,sizeof(cells));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int neighborsAlive = 0;
            for (int ny = (y - 1 + height) % height; ny != (y + 2 + height) % height; ny = (ny + 1) % height)
                for (int nx = (x - 1 + width) % width; nx != (x + 2 + width) % width; nx = (nx + 1) % width)
                    if (!(x == nx && y == ny)) neighborsAlive += cells[ny][nx];
            if ((!cells[y][x] && neighborsAlive == 3) || (cells[y][x] && (neighborsAlive == 2 || neighborsAlive == 3))) cellsUpdated[y][x] = true;
            else cellsUpdated[y][x] = false;
            board[boardIndex++] = cellsUpdated[y][x] ? 219 : ' ';//To have a single char represent a cell remove the other line and replace 219 with 'O'
            board[boardIndex++] = cellsUpdated[y][x] ? 219 : ' ';
        }
        board[boardIndex++] = '\n';
    }
    SetConsoleCursorPosition(hStdout, dest);
    printf("%s", board);
    bool matchFound = false;
    for (size_t i = 0; i < maxListSize; i++)
        if (memcmp(lastboards[i], board, sizeof(board)))
            matchFound = true;
    timesBoardsMatched = matchFound ? timesBoardsMatched + 1 : 0;
    if (timesBoardsMatched >= timesBoardsCanMatch) gameOver = true;
    memcpy(lastboards[(amountInArray = amountInArray++ % maxListSize)], board, sizeof(board));
    memcpy(cells, cellsUpdated, sizeof(cells));
}
int main()
{
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);
    SetConsoleCursorInfo(hStdout, &cursorinfo);
    //do
    //{
        gameOver = false;
        unsigned short newCell;
        newCell = 'O ' - ' ';
        newCell += 255;
        std::cout << char(newCell >> 8) << ' ';
        std::cout << unsigned short(65535 - (newCell << 8)) << ' ';
        //std::cout << std::bitset<64>(newCell) << ' ';
        //generateInitialCells();
        //while (!gameOver) {
            //Sleep(50);
            //updateAndRedrawCells();
        //}
    //} while (getchar() == '\n');
}
