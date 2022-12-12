// GoL_C++.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <windows.h>
#include <random>
#include <iostream>
#include <bitset>
const int width = 118;//To have a single char represent a cell multiply this by 2
const int height = 62;
const int chanceOfInitialCell = 10;
unsigned short cells[width * height];
bool gameOver = false;
const int maxListSize = 10;
int timesBoardsMatched = 0;
int timesBoardsCanMatch = 50;
unsigned short lastboards[maxListSize][width * height];//To have a single char represent a cell  multiply by 2
int amountInArray = 0;
const HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
const COORD dest = {0,0};
const CONSOLE_CURSOR_INFO cursorinfo = {1, 0};
void generateInitialCells() {
    std::random_device rand;
    for (size_t i = 0; i < width * height; i++)
        cells[i] = (rand() % 100 <= chanceOfInitialCell ? 'O ' : '  ') - ' ';
}
int countSetBits(unsigned short n) 
{
    return n == 0 ? 0 : 1 + countSetBits(n & (n - 1));
}
int clearBit(unsigned short n, unsigned short k)
{
    return (n | (~(1 << (k - 1))));
}
int setBit(unsigned short n, unsigned short k)
{
    return (n | (1 << (k - 1)));
}
void updateAndRedrawCells() {
    int boardIndex = 0;
    for (int i = 0; i < width * height; i++)
    {
        if (cells[i] >> 14 & 1)
            for (size_t j = 0; j < 8; j++) // Bits in Cardinal Direction NW,N,NE,W,E,SW,S,SE
            {
                switch (j)
                {
                    case 0:
                        cells[i - width - 1 % (width & height)] = setBit(cells[i - width - 1 % (width & height)], 8 - j);
                            break;
                    case 1:
                        cells[i - width % (width & height)] = setBit(cells[i - width % (width & height)], 8 - j);
                            break;
                    case 2:
                        cells[i - width + 1 % (width & height)] = setBit(cells[i - width + 1 % (width & height)], 8 - j);
                            break;
                    case 3:
                        cells[i - 1 % (width & height)] = setBit(cells[i - 1 % (width & height)], 8 - j);
                            break;
                    case 4:
                        cells[i + 1 % (width & height)] = setBit(cells[i + 1 % (width & height)], 8 - j);
                            break;
                    case 5:
                        cells[i + width - 1 % (width & height)] = setBit(cells[i + width - 1 % (width & height)], 8 - j);
                            break;
                    case 6:
                        cells[i + width % (width & height)] = setBit(cells[i + width % (width & height)], 8 - j);
                            break;
                    case 7:
                        cells[i + width + 1 % (width & height)] = setBit(cells[i + width + 1 % (width & height)], 8 - j);
                            break;
                    default:
                        break;
                }
            }
    }
    SetConsoleCursorPosition(hStdout, dest);
    char board[width * height + height];
    for (size_t i = 0; i < width * height; i++)
    {
        if (i % width == 0)
            board[i/width] = '\n';
        int neighborsAlive = countSetBits(unsigned short(cells[i] & 0x00FF));
        if ((!bool(cells[i] >> 1) && neighborsAlive == 3) || (bool(cells[i] >> 1) && (neighborsAlive == 2 || neighborsAlive == 3)))
            cells[i] = setBit(cells[i], 1);
        else
            cells[i]  = clearBit(cells[i], 1);
        board[i] = cells[i] >> 15 & 1 ? 'O' : '-';
    }
    std::cout << board;
    bool matchFound = false;
    for (size_t i = 0; i < maxListSize; i++)
        if (memcmp(lastboards[i], cells, sizeof(unsigned short)) == 0)
            matchFound = true;
    timesBoardsMatched = matchFound ? timesBoardsMatched + 1 : 0;
    if (timesBoardsMatched >= timesBoardsCanMatch) gameOver = true;
    memcpy(lastboards[(amountInArray = amountInArray++ % maxListSize)], cells, sizeof(unsigned short));
}
int main()
{
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);
    SetConsoleCursorInfo(hStdout, &cursorinfo);
    do
    {
        gameOver = false;
        generateInitialCells();
        while (!gameOver) {
            Sleep(50);
            updateAndRedrawCells();
        }
    } while (getchar() == '\n');
}
