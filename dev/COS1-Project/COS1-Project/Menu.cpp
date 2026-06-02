#include "Menu.h"
#include <string>
#include "Helper.h"
#include <algorithm>

//Title screen with menu (Testing now but add to GameManager)
void Menu::draw(int highScore)
{
	//int highScore = 765430;
	std::cout << "\033[2J\033[1;1H"; //clearscreen
	std::cout << BLUE << "##########################################################\n";
	std::cout << "#                                                        #\n";

	std::cout << "#     " << YELLOW << "######   ###   ####   ##   ##   ###   ##   ##" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "##   ## ## ## ##  ##  ### ###  ## ##  ###  ##" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "######  ##### ##      ## # ##  #####  #### ##" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "##      ## ## ##  ##  ##   ##  ## ##  ## ####" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "##      ## ##  ####   ##   ##  ## ##  ##  ###" << BLUE << "      #\n";

	std::cout << "#                                                        #\n";
	std::cout << "##########################################################\n";
	std::cout << "#                                                        #\n";

	std::cout << "#               ";
	if (currentSelection == MenuChoice::Start)
	{
		std::cout << YELLOW << "[1]  START GAME" << BLUE << "                          #\n";
	}else
	{
		std::cout << WHITE << "[1]  START GAME" << BLUE << "                          #\n";
	}
	std::cout << "#               "; 
	if (currentSelection == MenuChoice::HighScores)
	{ 
		std::cout << YELLOW << "[2] HIGH SCORES" << BLUE << "                          #\n";
	}
	else 
	{
		std::cout << WHITE << "[2] HIGH SCORES" << BLUE << "                          #\n";
	}
	std::cout << "#               "; 
	if (currentSelection == MenuChoice::Difficulty)
	{ 
		std::cout << YELLOW << "[3] DIFFICULTY" << BLUE << "                           #\n";
	}
	else 
	{
		std::cout << WHITE << "[3] DIFFICULTY" << BLUE << "                           #\n";
	}
	std::cout << "#               "; 
	if (currentSelection == MenuChoice::Exit)
	{ 
		std::cout << YELLOW << "[4] EXIT" << BLUE << "                                 #\n";
	}
	else 
	{
		std::cout << WHITE << "[4] EXIT" << BLUE << "                                 #\n";
	}

	std::cout << "#                                                        #\n";
	std::cout << "##########################################################\n";
	std::cout << "#                                                        #\n";
	std::cout << "#                                                        #\n";
	std::cout << "#                    "<< RED << "HIGH SCORE" << BLUE << "                          #\n";
	std::cout << "#                     " << WHITE << highScore << BLUE << "                             #\n";
	std::cout << "#                                                        #\n";
	std::cout << "##########################################################\n";
	std::cout << RESET;
}

MenuChoice Menu::getUserChoice()
{
	std::string input;
	 
	
	std::getline(std::cin, input);
	//std::transform(input.begin(), input.end(), input.begin(), ::tolower);

	if (input == "1")
	{
		currentSelection = MenuChoice::Start;
	}
	if (input == "2")
	{
		currentSelection = MenuChoice::HighScores;
	}
	if (input == "3")
	{
		currentSelection = MenuChoice::Difficulty;
	}
	if (input == "4")
	{
		currentSelection = MenuChoice::Exit;
	}

	return currentSelection;

	
	

	
}
