
#include "Map.h"
#include <windows.h>
#include <ctime>
#include <cmath>

void eatenByCat();
void getCheese();

int Map::getHeight() const {
    return HEIGHT;
}

int Map::getWidth() const {
    return WIDTH;
}

Map::Map(int width, int height) {
    srand(time(0));
    WIDTH = width;
    HEIGHT = height;

    maze = new Square*[HEIGHT];
    Square* cells = new Square[HEIGHT * WIDTH];
    for (int i = 0; i < HEIGHT; ++i) {
        maze[i] = cells + WIDTH * i;
    }
    generateMap();
    breakWall();
    //reveal edge
    for (int i = 0; i < HEIGHT; ++i) {
        maze[i][0].setVisible();
        maze[i][WIDTH - 1].setVisible();
    }
    for (int i = 0; i < WIDTH; ++i) {
        maze[0][i].setVisible();
        maze[HEIGHT - 1][i].setVisible();
    }
}

Map::~Map() {
    delete[] maze[0];
    delete[] maze;
}

Square& Map::getSquare(Coordinate coordinate) {
    return maze[coordinate.getY()][coordinate.getX()];
}

Square& Map::getSquare(int xCoordinate, int yCoordinate) {
    return maze[yCoordinate][xCoordinate];
}

void Map::revealAroundMouse(Coordinate coordinate) {
    int xCoordinate = coordinate.getX();
    int yCoordinate = coordinate.getY();
    for (int i = yCoordinate - 1; i <= yCoordinate + 1; ++i) {
        for (int j = xCoordinate - 1; j <= xCoordinate + 1; ++j) {
            maze[i][j].setVisible();
        }
    }
}

void Map::setAllVisible() {
    for (int i = 0; i < HEIGHT * WIDTH; ++i) {
        maze[0][i].setVisible();
    }
}

void Map::breakWall() {
    int numberOfWall = (WIDTH - 2) * (HEIGHT - 2) / 10;
    int xCoordinate;
    int yCoordinate;
    for (int i = 0; i < numberOfWall; i++) {
        bool isWall;
        bool bigCell;
        short wallAdjacent;
        do {
            xCoordinate = rand() % (WIDTH - 2) + 1;
            yCoordinate = rand() % (HEIGHT - 2) + 1;

            isWall = maze[yCoordinate][xCoordinate].isWall();
            bigCell = hasBigOpenCell(xCoordinate, yCoordinate);
            wallAdjacent = numberOfAdjacentWalls(xCoordinate, yCoordinate);
        } while (!isWall || wallAdjacent == 4 || bigCell);
        maze[yCoordinate][xCoordinate].setWall(false);
    }
}

bool Map::hasBigOpenCell(int xCoordinate, int yCoordinate) {
    if (!maze[yCoordinate - 1][xCoordinate].isWall()) {
        if (!maze[yCoordinate][xCoordinate - 1].isWall() && !maze[yCoordinate - 1][xCoordinate - 1].isWall()) {
            return true;
        }
        if (!maze[yCoordinate][xCoordinate + 1].isWall() && !maze[yCoordinate - 1][xCoordinate + 1].isWall()) {
            return true;
        }
    }
    if (!maze[yCoordinate + 1][xCoordinate].isWall()) {
        if (!maze[yCoordinate][xCoordinate - 1].isWall() && !maze[yCoordinate + 1][xCoordinate - 1].isWall()) {
            return true;
        }
        if (!maze[yCoordinate][xCoordinate + 1].isWall() && !maze[yCoordinate + 1][xCoordinate + 1].isWall()) {
            return true;
        }
    }
    return false;
}

short Map::numberOfAdjacentWalls(int xCoordinate, int yCoordinate) {
    short count = 0;
    if (xCoordinate > 0) {
        if (maze[yCoordinate][xCoordinate - 1].isWall()) {
            count++;
        }
    }
    if (xCoordinate < WIDTH - 1) {
        if (maze[yCoordinate][xCoordinate + 1].isWall()) {
            count++;
        }
    }
    if (yCoordinate > 0) {
        if (maze[yCoordinate - 1][xCoordinate].isWall()) {
            count++;
        }
    }
    if (yCoordinate < HEIGHT - 1) {
        if (maze[yCoordinate + 1][xCoordinate].isWall()) {
            count++;
        }
    }
    return count;
}

ostream &operator<<(ostream &os, const Map &printingMap) {
    for (int i = 0; i < printingMap.getHeight(); ++i) {
        for (int j = 0; j < printingMap.getWidth(); ++j) {
            os << printingMap.maze[i][j];
        }
        os << "\n";
    }
    return os;
}

void Map::generateMap() {
    Square** generationMaze = new Square*[HEIGHT + 2];
    Square* cells = new Square[(HEIGHT + 2) * (WIDTH + 2)];
    for (int i = 0; i < (HEIGHT + 2); ++i) {
        generationMaze[i] = cells + (WIDTH + 2) * i;
    }
    bool isBacktracking = false;

    for (int i = 0; i < (HEIGHT + 2); ++i) {
        generationMaze[i][0].setVisited();
        generationMaze[i][WIDTH + 1].setVisited();
    }
    for (int i = 0; i < (WIDTH + 2); ++i) {
        generationMaze[0][i].setVisited();
        generationMaze[HEIGHT + 1][i].setVisited();
    }

    Coordinate lastVisited(2, 2);
    generationMaze[2][2].setVisited();

    int numberOfSquaresShouldVisit = ceil((WIDTH  - 2) / 2.0) * ceil((HEIGHT - 2) / 2.0);

    if (WIDTH % 2 == 0) {
        for (int i = 0; i < (HEIGHT + 2); ++i) {
            generationMaze[i][WIDTH].setWall(true);
            if (i % 2 != 0) {
                generationMaze[i][WIDTH - 1].setWall(true);
            }
        }
    }

    if (HEIGHT % 2 == 0) {
        for (int i = 1; i <= WIDTH; i++) {
            generationMaze[HEIGHT][i].setWall(true);
            if (i % 2 != 0) {
                generationMaze[HEIGHT - 1][i].setWall(true);
            }
        }
    }

    Coordinate currentLocation(2, 2);
    vector<Coordinate> backTracker;

    int numberOfSquaresVisited = 1;
    while (numberOfSquaresVisited < numberOfSquaresShouldVisit) {
        vector<Coordinate> availableNextMoves = Square::suspect(currentLocation, lastVisited, generationMaze, isBacktracking);
        if (availableNextMoves.empty()) {
            currentLocation = backTracker.back();
            backTracker.pop_back();
            lastVisited = backTracker.back();
            // if doing backtracking, we don't build walls
            isBacktracking = true;
        } else {
            if (isBacktracking) {
                isBacktracking = false;
            }
            lastVisited = currentLocation;
            backTracker.push_back(lastVisited);
            currentLocation = availableNextMoves[rand() % availableNextMoves.size()];
            numberOfSquaresVisited += 1;
        }
        generationMaze[currentLocation.getY()][currentLocation.getX()].setVisited();
    }
    Square::suspect(currentLocation, lastVisited, generationMaze, isBacktracking);

    for (int i = 1; i <= HEIGHT; i++) {
        for (int j = 1; j <= WIDTH; j++) {
            maze[i - 1][j - 1] = generationMaze[i][j];
        }
    }

    delete[] cells;
    delete[] generationMaze;
}

Square::Square() {
    wall = false;
    mouse = false;
    catNumber = 0;
    isCheese = false;
    isVisible = false;
    isVisited = false;

    updateOutput();
}

void Square::setWall(bool isWall) {
    Square::wall = isWall;
    updateOutput();
}

bool Square::isWall() const {
    return wall;
}

bool Square::setMouse(bool isMouse) {
    if (wall) {
        return false;
    }
    Square::mouse = isMouse;
    updateOutput();
    statusCheck();
    return true;
}

bool Square::isMouse() const {
    return mouse;
}

void Square::setCheese(bool isCheese) {
    Square::isCheese = isCheese;
    updateOutput();
    statusCheck();
}

void Square::setVisible() {
    Square::isVisible = true;
    updateOutput();
}

void Square::addCat() {
    catNumber++;
    updateOutput();
    statusCheck();
}

void Square::removeCat() {
    catNumber--;
    updateOutput();
}

void Square::updateOutput() {
    if (mouse) {
        if (catNumber == 0) {
            output = '@';
            color = 9;
        } else {
            output = 'X';
            color = 4;
        }
    } else if (catNumber > 0) {
        output = '!';
        color = 4;
    } else if (isCheese) {
        output = '$';
        color = 6;
    } else if (!isVisible) {
        output = '.';
        color = 7;
    } else if (wall) {
        output = '#';
        color = 15;
    } else {
        output = ' ';
        color = 15;
    }
}

void Square::statusCheck() {
    if (catNumber > 0 && mouse) {
        eatenByCat();
    } else if (mouse && isCheese) {
        getCheese();
    }
}

ostream &operator<<(ostream &os, const Square &square) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),square.color);
    os << square.output;
    return os;
}

void Square::setVisited() {
    isVisited = true;
    updateOutput();
}

vector<Coordinate>
Square::suspect(Coordinate coordinate, Coordinate lastVisited, Square** generationMaze, bool isBacktracking) {
    vector<Coordinate> availableNextMoves;
    Coordinate leftCoordinate(coordinate.getX() - 2, coordinate.getY());
    Coordinate rightCoordinate(coordinate.getX() + 2, coordinate.getY());
    Coordinate upCoordinate(coordinate.getX(), coordinate.getY() - 2);
    Coordinate downCoordinate(coordinate.getX(), coordinate.getY() + 2);
    Square& left = generationMaze[leftCoordinate.getY()][leftCoordinate.getX()];
    Square& right = generationMaze[rightCoordinate.getY()][rightCoordinate.getX()];
    Square& up = generationMaze[upCoordinate.getY()][upCoordinate.getX()];
    Square& down = generationMaze[downCoordinate.getY()][downCoordinate.getX()];

    if (!leftCoordinate.isEqual(lastVisited) && !left.isWall()) {
        if (left.isVisited) {
            if (!isBacktracking) {
                generationMaze[leftCoordinate.getY()][leftCoordinate.getX() + 1].setWall(true);
                generationMaze[leftCoordinate.getY() - 1][leftCoordinate.getX() + 1].setWall(true);
                generationMaze[leftCoordinate.getY() + 1][leftCoordinate.getX() + 1].setWall(true);
            }
        } else {
            availableNextMoves.push_back(leftCoordinate);
        }
    }
    if (!rightCoordinate.isEqual(lastVisited) && !right.isWall()) {
        if (right.isVisited) {
            if (!isBacktracking) {
                generationMaze[rightCoordinate.getY()][rightCoordinate.getX() - 1].setWall(true);
                generationMaze[rightCoordinate.getY() - 1][rightCoordinate.getX() - 1].setWall(true);
                generationMaze[rightCoordinate.getY() + 1][rightCoordinate.getX() - 1].setWall(true);
            }
        } else {
            availableNextMoves.push_back(rightCoordinate);
        }
    }
    if (!upCoordinate.isEqual(lastVisited) && !up.isWall()) {
        if (up.isVisited) {
            if (!isBacktracking) {
                generationMaze[upCoordinate.getY() + 1][upCoordinate.getX()].setWall(true);
                generationMaze[upCoordinate.getY() + 1][upCoordinate.getX() - 1].setWall(true);
                generationMaze[upCoordinate.getY() + 1][upCoordinate.getX() + 1].setWall(true);
            }
        } else {
            availableNextMoves.push_back(upCoordinate);
        }
    }
    if (!downCoordinate.isEqual(lastVisited) && !down.isWall()) {
        if (down.isVisited) {
            if (!isBacktracking) {
                generationMaze[downCoordinate.getY() - 1][downCoordinate.getX()].setWall(true);
                generationMaze[downCoordinate.getY() - 1][downCoordinate.getX() - 1].setWall(true);
                generationMaze[downCoordinate.getY() - 1][downCoordinate.getX() + 1].setWall(true);
            }
        } else {
            availableNextMoves.push_back(downCoordinate);
        }
    }
    return availableNextMoves;
}
