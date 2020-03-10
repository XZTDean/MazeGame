#include <ostream>
#include "Map.h"
#include "Coordinate.h"
#include "UI.h"

Map* map;
class Record {
public:
    Record() {
        cat1 = Coordinate(-1, -1);
        cat2 = Coordinate(-1, -1);
        cat3 = Coordinate(-1, -1);
    }

    void setMouse(Coordinate mouse) {
        Record::mouse = mouse;
    }

    void setCat(Coordinate cat) {
        Coordinate tmp(-1, -1);
        if (cat1 == tmp) {
            cat1 = cat;
        } else if (cat2 == tmp) {
            cat2 = cat;
        } else if (cat3 == tmp) {
            cat3 = cat;
        }
    }

    void setCheese(Coordinate cheese) {
        Record::cheese = cheese;
    }

    void setCheeseNum(int cheeseCollected, int cheeseGoal) {
        Record::cheeseCollected = cheeseCollected;
        Record::cheeseGoal = cheeseGoal;
    }

    void set() const {
        map->getSquare(mouse).setMouse(true);
        map->getSquare(cat1).addCat();
        map->getSquare(cat2).addCat();
        map->getSquare(cat3).addCat();
        if (cheese != mouse) {
            map->getSquare(cheese).setCheese(true);
        }
    }

    void clear() const {
        map->getSquare(mouse).setMouse(false);
        map->getSquare(cat1).removeCat();
        map->getSquare(cat2).removeCat();
        map->getSquare(cat3).removeCat();
        map->getSquare(cheese).setCheese(false);
    }

    friend ostream& operator<<(ostream& os, const Record& record) {
        record.set();
        printMap(*map, record.cheeseCollected, record.cheeseGoal);
        record.clear();
        return os;
    }

private:
    Coordinate mouse;
    Coordinate cat1;
    Coordinate cat2;
    Coordinate cat3;
    Coordinate cheese;
    int cheeseCollected;
    int cheeseGoal;
};

vector<Record> gameRecord;

class Mouse {
public:
    Mouse() {
        coordinate = Coordinate(1, 1);
        map->getSquare(coordinate).setMouse(true);
        map->revealAroundMouse(coordinate);
        gameRecord.back().setMouse(coordinate);
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
        map->getSquare(previous).setMouse(false);
        if (!map->getSquare(coordinate).setMouse(true)) {
            coordinate = previous;
            map->getSquare(previous).setMouse(true);
            return false;
        }
        map->revealAroundMouse(coordinate);
        gameRecord.back().setMouse(coordinate);
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
        map->getSquare(coordinate).addCat();
        gameRecord.back().setCat(coordinate);
    }

    void moveCat() {
        char mouseDirection = chaseMouse();
        if (chaseMouse() != '\0') {
            move(mouseDirection);
        } else {
            vector<char> directions = findPath();
            if (directions.size() == 1) {
                move(directions.front());
            } else {
                char preDirection = coordinate.getDirection(previous);
                for (vector<char>::iterator it = directions.begin(); it < directions.end(); ++it) {
                    if (*it == preDirection) {
                        directions.erase(it);
                    }
                }
                move(directions[rand() % directions.size()]);
            }
        }
        gameRecord.back().setCat(coordinate);
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
        map->getSquare(previous).removeCat();
        map->getSquare(coordinate).addCat();
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
        if (!map->getSquare(up).isWall()) {
            directions.push_back('W');
        }
        if (!map->getSquare(down).isWall()) {
            directions.push_back('S');
        }
        if (!map->getSquare(right).isWall()) {
            directions.push_back('D');
        }
        if (!map->getSquare(left).isWall()) {
            directions.push_back('A');
        }
        return directions;
    }

    char chaseMouse() {
        if (map->getSquare(coordinate).isMouse()) {
            return 'X';
        }
        bool hasUp, hasDown, hasRight, hasLeft;
        Coordinate up = coordinate.clone();
        hasUp = true;
        Coordinate down = coordinate.clone();
        hasDown = true;
        Coordinate right = coordinate.clone();
        hasRight = true;
        Coordinate left = coordinate.clone();
        hasLeft = true;
        while (hasUp || hasDown || hasRight || hasLeft) {
            if (map->getSquare(up).isWall()) {
                hasUp = false;
            } else {
                up.decreaseY();
                if (map->getSquare(up).isMouse()) {
                    return 'W';
                }
            }
            if (map->getSquare(down).isWall()) {
                hasDown = false;
            } else {
                down.increaseY();
                if (map->getSquare(down).isMouse()) {
                    return 'S';
                }
            }
            if (map->getSquare(right).isWall()) {
                hasRight = false;
            } else {
                right.increaseX();
                if (map->getSquare(right).isMouse()) {
                    return 'D';
                }
            }
            if (map->getSquare(left).isWall()) {
                hasLeft = false;
            } else {
                left.decreaseX();
                if (map->getSquare(left).isMouse()) {
                    return 'A';
                }
            }
        }
        return '\0';
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
        xCoordinate = (rand() % (map->getWidth() - 2)) + 1;
        yCoordinate = (rand() % (map->getHeight() - 2)) + 1;
        if (!map->getSquare(xCoordinate, yCoordinate).isWall()) {
            valid = true;
        } else {
            continue;
        }
        for (int i = yCoordinate - 1; i <= yCoordinate + 1; ++i) {
            for (int j = xCoordinate - 1; j <= xCoordinate + 1; ++j) {
                if (map->getSquare(j, i).isMouse()) {
                    valid = false;
                }
            }
        }
    }
    cheese = new Coordinate(xCoordinate, yCoordinate);
    map->getSquare(*cheese).setCheese(true);
}

void getCheese() {
    cheeseCollected++;
    map->getSquare(*cheese).setCheese(false);
    if (cheeseCollected >= cheeseGoal) {
        gameEnd = true;
        map->setAllVisible();
        printMap(*map, cheeseCollected, cheeseGoal);
        cout << "Congratulations! You won!\n";
    } else {
        delete cheese;
        generateCheese();
    }
}

void eatenByCat() {
    cout << "I'm sorry, you have been eaten!\n";
    gameEnd = true;
    map->setAllVisible();
    printMap(*map, cheeseCollected, cheeseGoal);
    cout << "GAME OVER; please try again.\n";
}

char inputHandle() {
    char input;
    bool reInput;
    do {
        input = getInput();
        switch (input) {
            case 'M':
                map->setAllVisible();
                reInput = true;
                printMap(*map, cheeseCollected, cheeseGoal);
                break;
            case 'C':
                cheeseGoal = 1;
                reInput = true;
                printMap(*map, cheeseCollected, cheeseGoal);
                break;
            case '\0':
                reInput = true;
                printMap(*map, cheeseCollected, cheeseGoal);
                break;
            case '\1':
                reInput = true;
                printMap(*map, cheeseCollected, cheeseGoal);
                inputInvalid();
                break;
            default:
                reInput = false;
        }
    } while (reInput);
    return input;
}

void review() {
    for(vector<Record>::iterator it = gameRecord.begin(); it < gameRecord.end(); ++it) {
        cout << *it;
        Sleep(500);
    }
}

int main() {
    map = greeting();
    gameRecord.emplace_back();
    Mouse mouse;
    Cat* cats = new Cat[3];
    cats[0] = Cat(map->getWidth() - 2, 1);
    cats[1] = Cat(1, map->getHeight() - 2);
    cats[2] = Cat(map->getWidth() - 2, map->getHeight() - 2);
    generateCheese();
    gameRecord.back().setCheese(*cheese);
    gameRecord.back().setCheeseNum(cheeseCollected, cheeseGoal);

    char input;

    while (!gameEnd) {
        printMap(*map, cheeseCollected, cheeseGoal);
        bool isValidInput;
        gameRecord.emplace_back();

        //mouse move
        input = inputHandle();
        isValidInput = mouse.move(input);
        while (!isValidInput) {
            printMap(*map, cheeseCollected, cheeseGoal);
            moveInvalid();
            input = inputHandle();
            isValidInput = mouse.move(input);
        }

        //cat move
        for (int i = 0; i < 3; ++i) {
            cats[i].moveCat();
        }

        gameRecord.back().setCheese(*cheese);
        gameRecord.back().setCheeseNum(cheeseCollected, cheeseGoal);
    }

    char endGameInput = gameReview();
    switch (endGameInput) {
        case 'G':
        case 'g':
            gameRecord.back().clear();
            review();
            break;
    }

    delete cheese;
    delete[] cats;
    delete map;
    system("pause");
    return 0;
}
