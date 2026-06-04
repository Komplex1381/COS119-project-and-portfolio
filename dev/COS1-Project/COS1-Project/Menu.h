#pragma once
#include<iostream>
#include "MenuChoice.h"

class Menu
{
private:
	MenuChoice currentSelection = MenuChoice::Start;
	void drawLine(const std::string& text, const std::string& textColor, const std::string& borderColor) const;
public:
	void drawMenu(int highScore);
	void draw2();
	MenuChoice getUserChoice();

};

