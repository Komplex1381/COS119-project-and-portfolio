#include "Menu.h"
#include <string>
#include "Helper.h"
#include <conio.h> // for windows input
//#include <algorithm>


void Menu::drawLine(const std::string& text, const std::string& textColor, const std::string& borderColor) const
{
	int totalWidth = 56;
	int padding = totalWidth - static_cast<int>(text.length());

	std::cout << borderColor << "#" << textColor << text;
	std::cout << std::string(padding, ' ') << borderColor << " #\n";
}

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

void Menu::draw2()
{
	
	std::cout << "\033[2J\033[1;1H"; //clearscreen
	                                         

	std::cout << BLUE << "#" << (currentSelection == MenuChoice::Start ? YELLOW : WHITE) << "[1] START GAME" << BLUE << "#\n";
	std::cout << "#" << (currentSelection == MenuChoice::HighScores ? YELLOW : WHITE) << "[2] HIGH SCORES" << BLUE << "#\n";
	std::cout << "#" << (currentSelection == MenuChoice::Difficulty ? YELLOW : WHITE) << "[3] DIFFICULTY" << BLUE << "#\n";
	std::cout << "#" << (currentSelection == MenuChoice::Exit ? YELLOW : WHITE) << "[4] EXIT" << BLUE << "#\n";
	

	
}

MenuChoice Menu::getUserChoice()
{
	bool playerSelection = false;

	while (!playerSelection) 
	{
		//draw(highScore);

		int ch = _getch();

		//Windows arrow keys
		if (ch == 0 || ch == 224) 
		{
			ch = _getch();
			switch (ch)
			{
			case 72:
				ch = 'w'; //Up
				break;
			case 80:
				ch = 's'; //Down
				break;
			default:
				continue; //supposed to skip other keys
			}
		}
		switch (ch)
		{
		case 'w':
		case 'W':
			if (currentSelection == MenuChoice::Start) 
			{
				currentSelection = MenuChoice::Exit;
			}
			else 
			{
				currentSelection = static_cast<MenuChoice>(static_cast<int>(currentSelection) - 1);
			}
			break;
		case 's':
		case 'S':			
			if (currentSelection == MenuChoice::Exit)
			{
				currentSelection = MenuChoice::Start;
			}
			else 
			{
				currentSelection = static_cast<MenuChoice>(static_cast<int>(currentSelection) + 1);
			}
			break;
		case 13:
			playerSelection = true;
			break;
		default:
			break;
		}
		return currentSelection;
	}




	//std::string input;	 
	//
	//std::getline(std::cin, input);
	////std::transform(input.begin(), input.end(), input.begin(), ::tolower); supposed to make lower case, need to test out

	//if (input == "1")
	//{
	//	currentSelection = MenuChoice::Start;
	//}
	//if (input == "2")
	//{
	//	currentSelection = MenuChoice::HighScores;
	//}
	//if (input == "3")
	//{
	//	currentSelection = MenuChoice::Difficulty;
	//}
	//if (input == "4")
	//{
	//	currentSelection = MenuChoice::Exit;
	//}

	//return currentSelection;

	
	

	
}
