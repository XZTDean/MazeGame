#include <iostream>
#include <string>
#include "Map.h"
using namespace std;

#ifndef MAZEGAME_UI_H
#define MAZEGAME_UI_H

string instruction = "\nDIRECTIONS:\n\tFind 5 cheese before a cat eats you!"
                     "\nLEGEND:\n\t#: Wall\n\t@: You (a mouse)\n\t!: Cat\n\t$: Cheese\n\t.: Unexplored space"
                     "\nMOVES:\n\tUse W (up), A (left), S (down) and D (right) to move."
                     "\n\t(You must press enter after each move)."
                     "\nCat:\n\tCat will not move backward unless that is the only way."
                     "\n\tCat will track you if they can see you.";

Map* greeting() {
    int width;
    int height;
    cout << "----------------------------------------\n";
    cout << "Welcome to Cat and Mouse Maze Adventure!\n";
    cout << "----------------------------------------\n";
    cout << instruction << endl;

    char tmp[8];
    cout << "\nChoose the map size: (at least 10*10)\n";
    do {
        width = 20;
        cout << "\tWidth: ";
        cout << "\033[90m20\033[0m\b\b";
        cin.getline(tmp, 8);
        if (tmp[0] >= '0' && tmp[0] <= '9') {
            width = stoi(tmp);
        }
    } while (width < 10);
    do {
        height = 15;
        cout << "\tHeight: ";
        cout << "\033[90m15\033[0m\b\b";
        cin.getline(tmp, 8);
        if (tmp[0] >= '0' && tmp[0] <= '9') {
            height = stoi(tmp);
        }
    } while (height < 10);

    return new Map(width, height);
}

void printMap(const Map& map, int cheeseCollected, int cheeseGoal) {
    system("clear");
    cout << "Maze:\n" << map;
    cout << "Cheese collected: " << cheeseCollected << " of " << cheeseGoal << endl;
}

char getInput() {
    char input;
    cout << "Enter your move [WASD?]: ";
    cin >> input;
    input = toupper(input);
    cin.sync();
    char correctInputs[] = {'W', 'A', 'S', 'D', 'M', 'C'};
    for (char correctInput: correctInputs) {
        if (correctInput == input) {
            return input;
        }
    }
    if (input == '?') {
        system("clear");
        cout << instruction << endl;
        cin.get();
        return '\0';
    }
    return '\1';
}

void inputInvalid() {
    cout << "Invalid move. Please enter just A (left), S (down), D (right), or W (up).\n";
}

void moveInvalid() {
    cout << "Invalid move: you cannot move through walls!\n";
}

char gameReview() {
    char tmp[8];
    cout << "\nPress G to review the whole game, R to restart, empty to exit\n";
    cout << "\033[90m_\033[0m\b";
    cin.getline(tmp, 8);
    return tmp[0];
}

void printLose() {
    cout << "I'm sorry, you have been eaten!\n";
    cout << "GAME OVER; please try again.\n";
}

void printWin() {
    cout << "Congratulations! You won!\n";
}

#endif //MAZEGAME_UI_H
