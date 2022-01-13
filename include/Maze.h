#pragma once

enum WALL {
    GATE,
    HORIZONTAL,
    VERTICAL,
    CORNER,
    CUBE
};
class Maze
{
private:
    int width;
    int height;
    int **visited;
    int **verticalWalls;
    int **horizontalWalls;
    WALL **maze;
    int gate;

public:
    Maze(int width);
    ~Maze();
    void generate(int x, int y);
    int adjency(int dir[], int x, int y);
    WALL **getMaze() const { return maze; }; 
    void create();
    void print();
};