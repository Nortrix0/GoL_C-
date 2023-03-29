// GoL_C++.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <windows.h>
#include <stdio.h>
const int width = 118*2;//To have a single char represent a cell multiply this by 2
const int height = 62;
bool cells[height][width];
bool cellsUpdated[height][width];
char board[width * 2 * height + height];//To have a single char represent a cell remove the multiply by 2
int main()
{
    for (unsigned short i = 0; i < width * height; i++)
        cells[i % height][i / height] = rand() % 100 <= 10;
    while (true) {
        int boardIndex = 0;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
            {
                int neighborsAlive = 0;
                for (int ny = (y - 1 + height) % height; ny != (y + 2 + height) % height; ny = (ny + 1) % height)
                    for (int nx = (x - 1 + width) % width; nx != (x + 2 + width) % width; nx = (nx + 1) % width)
                        if (!(x == nx && y == ny)) neighborsAlive += cells[ny][nx];
                cellsUpdated[y][x] = neighborsAlive == 3 || (cells[y][x] && neighborsAlive == 2);
                board[boardIndex++] = cellsUpdated[y][x] ? 'O' : ' ';//To have a single char as a cell replace 219 with 'O'
                if (x + 1 == width)
                    board[boardIndex++] = '\n';
            }
        printf("\33[?25l\33[8;%i;%it\33[;H%s", (height + 1), (width * 2 + 1), board);//Turn Cursor Invisible and Set height and width, Set Cursor Position to Top and Print board
        memcpy(cells, cellsUpdated, sizeof(cells));
    }
}
