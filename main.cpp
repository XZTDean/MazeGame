#include "Map.h"
#include "Coordinate.h"
#include "UI.h"

Map map;

class Mouse {
public:
    Mouse() {
        coordinate = Coordinate(1, 1);
        map.getSquare(coordinate).setMouse(true);
        map.revealAroundMouse(coordinate);
    }

    bool move(char direction) {
        Coordinate previous = coordinate.clone();
        switch (direction) {
            case 'W':
                coordinate.decreaseY();
                break;
            case 'S':
                coordinate.increaseY();
                break;
            case 'A':
                coordinate.decreaseX();
                break;
            case 'D':
                coordinate.increaseX();
                break;
        }
        if (!map.getSquare(coordinate).setMouse(true)) {
            return false;
        }
        map.getSquare(previous).setMouse(false);
        map.revealAroundMouse(coordinate);
        return true;
    }

private:
    Coordinate coordinate;
};

class Cat {
public:
    Cat() = default;

    Cat(int xCoordinate, int yCoordinate) {
        coordinate = Coordinate(xCoordinate, yCoordinate);
        previous = Coordinate(xCoordinate, yCoordinate);
        map.getSquare(coordinate).addCat();
    }

    void moveCat() {
        vector<char> directions = findPath();
        if (directions.size() == 1) {
            move(directions.front());
        } else {
            char preDirection = coordinate.getDirection(previous);
            for (vector<char>::iterator it = directions.begin(); it != directions.end(); ++it) {
                if (*it == preDirection) {
                    directions.erase(it);
                }
            }
            move(directions[rand() % directions.size()]);
        }
    }

private:
    void move(char direction) {
        previous = coordinate.clone();
        switch (direction) {
            case 'W':
                coordinate.decreaseY();
                break;
            case 'S':
                coordinate.increaseY();
                break;
            case 'A':
                coordinate.decreaseX();
                break;
            case 'D':
                coordinate.increaseX();
                break;
        }
        map.getSquare(previous).removeCat();
        map.getSquare(coordinate).addCat();
    }

    vector<char> findPath(){
        vector<char> directions;
        Coordinate up = coordinate.clone();
        up.decreaseY();
        Coordinate down = coordinate.clone();
        down.increaseY();
        Coordinate right = coordinate.clone();
        right.increaseX();
        Coordinate left = coordinate.clone();
        left.decreaseX();
        if (!map.getSquare(up).isWall()) {
            directions.push_back('W');
        }
        if (!map.getSquare(down).isWall()) {
            directions.push_back('S');
        }
        if (!map.getSquare(right).isWall()) {
            directions.push_back('D');
        }
        if (!map.getSquare(left).isWall()) {
            directions.push_back('A');
        }
        return directions;
    }

    Coordinate coordinate;
    Coordinate previous;
};

bool gameEnd = false;
int cheeseCollected = 0;
int cheeseGoal = 5;
Coordinate* cheese;

void generateCheese() {
    bool valid = false;
    int xCoordinate;
    int yCoordinate;
    while (!valid) {
        xCoordinate = (rand() % (map.getWidth() - 2)) + 1;
        yCoordinate = (rand() % (map.getHeight() - 2)) + 1;
        if (!map.getSquare(xCoordinate, yCoordinate).isWall()) {
            valid = true;
        } else {
            continue;
        }
        for (int i = yCoordinate - 1; i <= yCoordinate + 1; ++i) {
            for (int j = xCoordinate - 1; j <= xCoordinate + 1; ++j) {
                if (map.getSquare(j, i).isMouse()) {
                    valid = false;
                }
            }
        }
    }
    cheese = new Coordinate(xCoordinate, yCoordinate);
    map.getSquare(*cheese).setCheese(true);
}

void getCheese() {
    cheeseCollected++;
    map.getSquare(*cheese).setCheese(false);
    generateCheese();
    if (cheeseCollected >= cheeseGoal) {
        gameEnd = true;
        cout << "Congratulations! You won!\n";
        map.setAllVisible();
        printMap(map, cheeseCollected, cheeseGoal);
    }
}

void eatenByCat() {
    cout << "I'm sorry, you have been eaten!\n";
    gameEnd = true;
    map.setAllVisible();
    printMap(map, cheeseCollected, cheeseGoal);
    cout << "GAME OVER; please try again.\n";
}

char inputHandle() {
    char input;
    bool isCheat;
    do {
        input = getInput();
        if (input == 'M') {
            map.setAllVisible();
            isCheat = true;
        } else if (input == 'C') {
            cheeseGoal = 1;
            isCheat = true;
        } else {
            isCheat = false;
        }
    } while (isCheat);
    return input;
}

int main() {
    Mouse mouse;
    Cat* cats = new Cat[3];
    cats[0] = Cat(map.getWidth() - 2, 1);
    cats[1] = Cat(1, map.getHeight() - 2);
    cats[2] = Cat(map.getWidth() - 2, map.getHeight() - 2);
    generateCheese();
    char input;

    printHeading();
    while (!gameEnd) {
        printMap(map, cheeseCollected, cheeseGoal);
    }
    delete[] cats;
    return 0;
}
