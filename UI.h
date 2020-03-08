#include <iostream>
#include <string>
#include "Map.h"
using namespace std;

#ifndef MAZEGAME_UI_H
#define MAZEGAME_UI_H

string instruction = "\nDIRECTIONS:\n\tFind 5 cheese before a cat eats you!"
                     "\nLEGEND:\n\t#: Wall\n\t@: You (a mouse)\n\t!: Cat\n\t$: Cheese\n\t.: Unexplored space"
                     "\nMOVES:\n\tUse W (up), A (left), S (down) and D (right) to move."
                     "\n\t(You must press enter after each move).";

void printHeading() {
    cout << "----------------------------------------\n";
    cout << "Welcome to Cat and Mouse Maze Adventure!\n";
    cout << "----------------------------------------\n";
    cout << instruction << endl;
}

void printMap(Map map, int cheeseCollected, int cheeseGoal) {
    system("clear");
    cout << "Maze:\n" << map;
    cout << "Cheese collected: " << cheeseCollected << " of " << cheeseGoal << endl;
}

bool inputCheck(char input) {
    char correctInputs[] = {'W', 'A', 'S', 'D', 'M', 'C'};
    for (char correctInput: correctInputs) {
        if (correctInput == input) {
            return true;
        }
    }
    if (input == '?') {
        cout << instruction << endl;
        return false;
    }
    cout << "Invalid move. Please enter just A (left), S (down), D (right), or W (up).\n";
    return false;
}

char getInput() {
    char input;
    do {
        cout << "Enter your move [WASD?]: ";
        cin >> input;
        input = toupper(input);
    } while (!inputCheck(input));
    return input;
}

void moveInvalid() {
    cout << "Invalid move: you cannot move through walls!\n";
}

#endif //MAZEGAME_UI_H
