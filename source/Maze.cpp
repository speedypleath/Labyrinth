#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <Maze.h>

enum {
    North,
    East,
    South,
    West,
    NDir
};

Maze::Maze(int width)
{
    this->gate = rand() % width - 2;
    srand(time(NULL));
    this->width = width;
    visited = new int *[width];
    verticalWalls = new int *[width];
    horizontalWalls = new int *[width];
    maze = new WALL *[width + 2];
    for (int i = 0; i < width; i++)
    {
        visited[i] = new int[width];
        verticalWalls[i] = new int[width];
        horizontalWalls[i] = new int[width];
        for (int j = 0; j < width; j++)
        {
            visited[i][j] = 0;
        }
    }

    for (int i = 0; i < width + 2; i++)
    {
        maze[i] = new WALL[width + 2];
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
    for (int i = 0; i < width + 2; i++)
    {
        delete[] maze[i];
    }
    delete[] maze;
    delete[] visited;
    delete[] verticalWalls;
    delete[] horizontalWalls;
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

void Maze::create()
{
    int k = 0, n = width + 2;

    maze[k / n][k++ % n] = HORIZONTAL;

    for (int i = 0; i < width; i++)
    {
        if(i == gate)
            maze[k / n][k++ % n] = GATE;   
        else
            maze[k / n][k++ % n] = VERTICAL;
    }

    maze[k / n][k++ % n] = CORNER;

    for (int i = 0; i < width; i++) {
        maze[k / n][k++ % n] = HORIZONTAL;
        for (int j = 0; j < width; j++) {
            std :: cout << "i = " << i << " j = " << j << " k = " << k << " val:" << maze[(k-1) / n][ (k-1) % n ]<<std :: endl;
            if (horizontalWalls[i][j] == 1) {
                maze[k / n][k++ % n] = HORIZONTAL;
                continue;
            }
            if (verticalWalls[i][j] == 1) {
                maze[k / n][k++ % n] = VERTICAL;
                continue;
            }
            if (verticalWalls[i][j] == 1 && horizontalWalls[i][j] == 1) {
                maze[k / n][k++ % n] = CORNER;
                continue;
            }
            maze[k / n][k++ % n] = GATE;
        }
        maze[k / n][k++ % n] = HORIZONTAL;
    }

    maze[k / n][k++ % n] = CUBE;

    for (int i = 0; i < width; i++)
    {
        maze[k / n][k++ % n] = VERTICAL;
    }

    maze[k / n][k++ % n] = VERTICAL;
    
}

void Maze::print(){
    int i, j, k = 0, n = 2 * width + 1;

    std :: cout << '_';

    for (i = 0; i < width; i++)
    {
        if(i == gate){
            std :: cout << ' ';
            std :: cout << ' ';
        }
        else{
            std :: cout << '_';
            std :: cout << '_';
        }
    }
    std :: cout << '\n';
    for (i = 0; i < width - 1; i++)
    {
        std :: cout << '|';

        for (j = 0; j < width; j++)
        {
            if(i < width - 1 && verticalWalls[i][j])
                std :: cout << ' ';
            else
                std :: cout << '_';
            
            if(j < width - 1 && horizontalWalls[i][j])
                std :: cout << ' ';
            else 
                std :: cout << '|';
        }
        std :: cout << '\n';
    }

    std :: cout << '|';

    for (i = 0; i < width - 1; i++)
    {
        std :: cout << '_';
        std :: cout << '_';
    }

    std :: cout << '_';
    std :: cout << '|';
    std :: cout << '\n';
}