// COS1-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "Menu.h"
#include "MapLoader.h"
#include "Map.h"


bool gameOver = false;

int main()
{
   
    //Clear screen
    //std::cout << "\033[2J\033[1;1H"; //clearscreen
    Menu gameMenu;
    Map level1 = MapLoader::loadMap("lvl2.txt");
    //Map level1 = MapLoader::loadMap("level1.txt");

    //Not to make map bigger maybe change font size, research how.
    //level1.renderASCII();

    
    
    //gameMenu.draw2();

    while (!gameOver) 
    {
        
        //Draw
        gameMenu.draw(700);
        
        //Input
        gameMenu.getUserChoice();
        if (gameMenu.getUserChoice() == MenuChoice::Exit)
            gameOver = true;
        //Update & Collision detection MenuChoice::Exit

        //Draw?
        gameMenu.draw(700);
    }
}


