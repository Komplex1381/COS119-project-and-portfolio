// COS1-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "Menu.h"

bool gameOver = false;

int main()
{
    //Clear screen
    std::cout << "\033[2J\033[1;1H"; //clearscreen
    Menu gameMenu;

    //gameMenu.draw(765430);

    while (!gameOver) 
    {
        //Draw
        gameMenu.draw(765430);
        //Input

        //Update & Collision detection

        //Draw?
        gameMenu.draw(765430);
    }
}


