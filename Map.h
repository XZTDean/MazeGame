#include <ostream>
#include <vector>
#include "Coordinate.h"
using namespace std;

#ifndef MAZEGAME_MAP_H
#define MAZEGAME_MAP_H


class Square {
public:
    Square();

    void setWall(bool isWall);

    bool isWall() const;

    bool setMouse(bool isMouse);

    bool isMouse() const;

    void addCat();

    void removeCat();

    void setCheese(bool isCheese);

    void setVisible();

    void setVisited();

    static vector<Coordinate>
    suspect(Coordinate coordinate, Coordinate lastVisited, Square** generationMaze, bool isBacktracking);

    friend ostream &operator<<(ostream &os, const Square &square);

private:
    void updateOutput();

    void statusCheck();

    bool wall;
    bool mouse;
    bool isCheese;
    bool isVisible;
    short catNumber;
    bool isVisited; //used in generate map
    short color;
    char output;
};

class Map {
public:
    Map(int width, int height);

    ~Map();

    int getHeight() const;

    int getWidth() const;

    Square& getSquare(Coordinate coordinate);

    Square& getSquare(int xCoordinate, int yCoordinate);

    void revealAroundMouse(Coordinate coordinate);

    void setAllVisible();

    friend ostream &operator<<(ostream &os, const Map &map);

private:
    void generateMap();

    void breakWall();

    bool hasBigOpenCell(int xCoordinate, int yCoordinate);

    short numberOfAdjacentWalls(int xCoordinate, int yCoordinate);

    int HEIGHT;
    int WIDTH;
    Square** maze;
};

#endif //MAZEGAME_MAP_H
