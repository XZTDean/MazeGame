
#ifndef MAZEGAME_COORDINATE_H
#define MAZEGAME_COORDINATE_H


class Coordinate {
public:
    Coordinate() = default;

    Coordinate(int xCoordinate, int yCoordinate) : xCoordinate(xCoordinate), yCoordinate(yCoordinate) {}

    int getX() const {
        return xCoordinate;
    }

    int getY() const {
        return yCoordinate;
    }

    void increaseX() {
        xCoordinate += 1;
    }

    void increaseY() {
        yCoordinate += 1;
    }

    void decreaseX() {
        xCoordinate -= 1;
    }

    void decreaseY() {
        yCoordinate -= 1;
    }

    Coordinate clone() {
        return {xCoordinate, yCoordinate};
    }

    char getDirection(Coordinate otherCoordinate) {
        if (otherCoordinate.xCoordinate > this->xCoordinate) {
            return 'D';
        } else if (otherCoordinate.xCoordinate < this->xCoordinate) {
            return 'A';
        } else if (otherCoordinate.yCoordinate > this->yCoordinate) {
            return 'S';
        } else if (otherCoordinate.yCoordinate < this->yCoordinate) {
            return 'W';
        } else {
            return 'X';
        }
    }

    bool operator==(const Coordinate& rhs) const {
        return xCoordinate == rhs.xCoordinate &&
               yCoordinate == rhs.yCoordinate;
    }

    bool operator!=(const Coordinate& rhs) const {
        return !(rhs == *this);
    }

private:
    int xCoordinate{};
    int yCoordinate{};
};


#endif //MAZEGAME_COORDINATE_H
