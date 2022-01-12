#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <Maze.h>

enum
{
    North,
    East,
    South,
    West,
    NDir
};


Maze::Maze(int width)
{
    srand(time(NULL));
    this->width = width;
    visited = new int *[width];
    verticalWalls = new int *[width - 1];
    horizontalWalls = new int *[width];
    for (int i = 0; i < width; i++)
    {
        visited[i] = new int[width];
        verticalWalls[i] = new int[width];
        horizontalWalls[i] = new int[width - 1];
        for (int j = 0; j < width; j++)
        {
            visited[i][j] = 0;
        }
    }
}

Maze::~Maze()
{
    for (int i = 0; i < width; i++)
    {
        delete[] visited[i];
        delete[] verticalWalls[i];
        delete[] horizontalWalls[i];
    }
    delete[] visited;
    delete[] verticalWalls;
    delete[] horizontalWalls;
}

int Maze::countVerticalWalls(){
    int count = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (verticalWalls[i][j] == 1)
            {
                count++;
            }
        }
    }
    return count;
}

int Maze::countHorizontalWalls(){
    int count = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (horizontalWalls[i][j] == 1)
            {
                count++;
            }
        }
    }
    return count;
}

int Maze::adjency(int dir[], int x, int y)
{
    int ndir = 0;

    if (y > 0 && visited[y - 1][x] == 0)
        dir[ndir++] = North;
    if (x < width - 1 && visited[y][x + 1] == 0)
        dir[ndir++] = East;
    if (y < width - 1 && visited[y + 1][x] == 0)
        dir[ndir++] = South;
    if (x > 0 && visited[y][x - 1] == 0)
        dir[ndir++] = West;

    return ndir;
}

void Maze::generate(int x, int y)
{
    int dir[NDir];
    int ndir;
 
    visited[y][x] = 1;
 
    ndir = adjency(dir, x, y);
    while (ndir)
    {
        int nextDir = rand() % ndir;

        switch (dir[nextDir])
        {
        case North:
            verticalWalls[y - 1][x] = 1;
            generate(x, y - 1);
            break;
        case East:
            horizontalWalls[y][x] = 1;
            generate(x + 1, y);
            break;
        case South:
            verticalWalls[y][x] = 1;
            generate(x, y + 1);
            break;
        case West:
            horizontalWalls[y][x - 1] = 1;
            generate(x - 1, y);
            break;
        }

        ndir = adjency(dir, x, y);
    }
}

void Maze::print()
{
    // print horizontal walls
    printf("Horizontal Walls:\n");
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (horizontalWalls[i][j] == 1)
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }

    // print vertical walls
    printf("Vertical Walls:\n");
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (verticalWalls[i][j] == 1)
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }

    int i, j;

    for (i = 0; i < width; i++)
    {
        putchar('_');
        putchar('_');
    }

    putchar('\n');

    for (j = 0; j < width; j++)
    {
        putchar('|');

        for (i = 0; i < width; i++)
        {
            putchar(j < width - 1 && verticalWalls[j][i] ? ' ' : '_');
            putchar(i < width - 1 && horizontalWalls[j][i] ? ' ' : '|');
        }

        putchar('\n');
    }
}