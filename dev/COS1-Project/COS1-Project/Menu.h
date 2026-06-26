#pragma once
#include<iostream>
#include <fstream>
#include "MenuChoice.h"
#include <string>
#include "Helper.h"
#include <conio.h> // for windows input
#include <Windows.h>

class Menu
{
private:
	MenuChoice currentSelection = MenuChoice::Start;
	int mScoretoShow = 0;

	//Testing out padding that I saw online. Not doing what exactly I want but can be tweaked. Save for after game runs.
	void drawLine(const std::string& text, const std::string& textColor, const std::string& borderColor) const;
public:
	void drawMenu();
	//void drawInvinvicibleMenu();
	void setHighScore(int score);
	//draw to test other ideas as I figure out how to upgrade from ASCII 
	void draw2();
	MenuChoice getUserChoice();

};

