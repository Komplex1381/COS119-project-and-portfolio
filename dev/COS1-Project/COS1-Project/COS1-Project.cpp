// COS1-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "GameManager.h"
#include <crtdbg.h>
#define MEMORY_LEAK_LINE -1




bool gameOver = false;

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(MEMORY_LEAK_LINE); // DO NOT COMMENT OUT THIS LINE

    GameManager gameEngine;
    gameEngine.run();
   
}


