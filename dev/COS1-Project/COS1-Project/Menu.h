#pragma once
#include<iostream>
#include "MenuChoice.h"

class Menu
{
private:
	MenuChoice currentSelection = MenuChoice::Start;
public:
	void draw(int highScore);
	MenuChoice getUserChoice();

};

