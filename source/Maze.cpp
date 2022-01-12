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
    this->gate = rand() % (width - 2) + 1;
    srand(time(NULL));
    this->width = width;
    visited = new int *[width];
    verticalWalls = new int *[width];
    horizontalWalls = new int *[width];
    maze = new char *[width + 1];
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

    for (int i = 0; i < width + 1; i++)
    {
        maze[i] = new char[2 * width + 1];
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
    for (int i = 0; i < width + 1; i++)
    {
        delete[] maze[i];
    }
    delete[] maze;
    delete[] visited;
    delete[] verticalWalls;
    delete[] horizontalWalls;
}

int Maze::countBlocks(){
    int count = 0;
    for(int i = 0; i < width + 1; i++)
        for(int j = 0; j < 2 * width + 1; j++)
            if(maze[i][j] == '_' || maze[i][j] == '|')
                count++;
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

void Maze::create()
{
    int i, j, k = 0, n = 2 * width + 1;

    this->maze[k / n][k++ % n] = '_';

    for (i = 0; i < width; i++)
    {
        if(i == gate){
            this->maze[k / n][k++ % n] = ' ';
            this->maze[k / n][k++ % n] = ' ';
        }
        else{
            this->maze[k / n][k++ % n] = '_';
            this->maze[k / n][k++ % n] = '_';
        }
    }
    
    for (i = 0; i < width - 1; i++)
    {
        this->maze[k / n][k++ % n] = '|';

        for (j = 0; j < width; j++)
        {
            if(i < width - 1 && verticalWalls[i][j])
                this->maze[k / n][k++ % n] = ' ';
            else
                this->maze[k / n][k++ % n] = '_';
            
            if(j < width - 1 && horizontalWalls[i][j])
                this->maze[k / n][k++ % n] = ' ';
            else 
                this->maze[k / n][k++ % n] = '|';
        }
    }

    this->maze[k / n][k++ % n] = '|';

    for (i = 0; i < width - 1; i++)
    {
        this->maze[k / n][k++ % n] = '_';
        this->maze[k / n][k++ % n] = '_';
    }

    this->maze[k / n][k++ % n] = '_';
    this->maze[k / n][k++ % n] = '|';

}

void Maze::print(){
    for(int i = 0; i < width + 1; i++){
        for(int j = 0; j < 2 * width + 1; j++){
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}