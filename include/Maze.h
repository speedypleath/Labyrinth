#pragma once
class Maze
{
private:
    int width;
    int height;
    int **visited;
    int **verticalWalls;
    int **horizontalWalls;
    char **maze;
    int gate;

public:
    Maze(int width);
    ~Maze();
    void generate(int x, int y);
    int adjency(int dir[], int x, int y);
    int countVerticalWalls();
    int countHorizontalWalls();
    int **getVerticalWalls() const { return verticalWalls; }
    int **getHorizontalWalls() const { return horizontalWalls; }
    void create();
    void print();
};