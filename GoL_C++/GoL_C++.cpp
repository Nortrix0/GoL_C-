// GoL_C++.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <chrono>
#include <thread>
const int width = 118;//To have a single char represent a cell multiply this by 2
const int height = 62;
const int chanceOfInitialCell = 10;
bool cells[height][width];
bool cellsUpdated[height][width];
bool gameOver = false;
const int maxListSize = 10;
int timesBoardsMatched = 0;
int timesBoardsCanMatch = 50;
char lastboards[maxListSize][width*2 * height + height];//To have a single char represent a cell remove the multiply by 2
char board[width * 2 * height + height];//To have a single char represent a cell remove the multiply by 2
int amountInArray = 0;
int main()
{
    printf("\33[?25l");//Turn Cursor Invisible
    printf("\33[8;63;237t");
    do
    {
        gameOver = false;
        for (unsigned short i = 0; i < width * height; i++)
            cells[i % height][i / height] = rand() % 100 <= chanceOfInitialCell;
        while (!gameOver) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            int boardIndex = 0;
            memcpy(cellsUpdated, cells, sizeof(cells));
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int neighborsAlive = 0;
                    for (int ny = (y - 1 + height) % height; ny != (y + 2 + height) % height; ny = (ny + 1) % height)
                        for (int nx = (x - 1 + width) % width; nx != (x + 2 + width) % width; nx = (nx + 1) % width)
                            if (!(x == nx && y == ny)) neighborsAlive += cells[ny][nx];
                    cellsUpdated[y][x] = neighborsAlive == 3 || (cells[y][x] && neighborsAlive == 2);
                    board[boardIndex++] = cellsUpdated[y][x] ? 219 : ' ';//To have a single char as a cell remove the other line and replace 219 with 'O'
                    board[boardIndex++] = cellsUpdated[y][x] ? 219 : ' ';
                }
                board[boardIndex++] = '\n';
            }
            printf("\33[;H");//Set Cursor Position to Top
            printf("%s", board);
            bool matchFound = false;
            for (size_t i = 0; i < maxListSize; i++)
                if (memcmp(lastboards[i], board, sizeof(board)) == 0)
                    matchFound = true;
            timesBoardsMatched = matchFound ? timesBoardsMatched + 1 : 0;
            if (timesBoardsMatched >= timesBoardsCanMatch) gameOver = true;
            memcpy(lastboards[(amountInArray = amountInArray++ % maxListSize)], board, sizeof(board));
            memcpy(cells, cellsUpdated, sizeof(cells));
        }
    } while (getchar() == '\n');
}
