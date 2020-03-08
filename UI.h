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

void printMap(const Map& map, int cheeseCollected, int cheeseGoal) {
    system("CLS");
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
        system("CLS");
        cout << instruction << endl;
        system("pause");
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

#endif //MAZEGAME_UI_H
