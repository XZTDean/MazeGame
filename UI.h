#include <iostream>
#include <string>
#include "Map.h"
#include <windows.h>
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
    int width = 20;
    int height = 15;
    cout << "----------------------------------------\n";
    cout << "Welcome to Cat and Mouse Maze Adventure!\n";
    cout << "----------------------------------------\n";
    cout << instruction << endl;

    char tmp[8];
    cout << "\nChoose the map size: (at least 10*10)\n";
    do {
        cout << "\tWidth: ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "20\b\b";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        cin.getline(tmp, 8);
        if (tmp[0] >= '0' && tmp[0] <= '9') {
            width = stoi(tmp);
        }
    } while (width < 10);
    do {
        cout << "\tHeight: ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "15\b\b";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        cin.getline(tmp, 8);
        if (tmp[0] >= '0' && tmp[0] <= '9') {
            height = stoi(tmp);
        }
    } while (height < 10);

    return new Map(width, height);
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
