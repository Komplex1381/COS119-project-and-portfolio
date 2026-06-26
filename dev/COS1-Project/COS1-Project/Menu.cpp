#include "Menu.h"




void Menu::drawLine(const std::string& text, const std::string& textColor, const std::string& borderColor) const
{
	int totalWidth = 56;
	int padding = totalWidth - static_cast<int>(text.length() + 6);

	std::cout << borderColor << "#\t\t    " << textColor << text;
	std::cout << std::string(padding, ' ') << borderColor << " #\n";
}

//Title screen with menu (Testing now but add to GameManager)
void Menu::drawMenu()
{
	COORD coord = { 0,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	std::string scoreStr = std::to_string(mScoretoShow);
	int scoreLegnth = static_cast<int>(scoreStr.length());
	int totalWidth = 31;//28
	int padding = totalWidth - scoreLegnth;

	if (padding < 0)
	{
		padding = 0;
	}
	

	
	std::cout << BLUE << "##########################################################\n"; //58
	std::cout << "#                                                        #\n";

	std::cout << "#     " << YELLOW << "######   ###   ####   ##   ##   ###   ##   ##" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "##   ## ## ## ##  ##  ### ###  ## ##  ###  ##" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "######  ##### ##      ## # ##  #####  #### ##" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "##      ## ## ##  ##  ##   ##  ## ##  ## ####" << BLUE << "      #\n";
	std::cout << "#     " << YELLOW << "##      ## ##  ####   ##   ##  ## ##  ##  ###" << BLUE << "      #\n";

	std::cout << "#                                                        #\n";
	std::cout << "##########################################################\n";
	std::cout << "#                                                        #\n";

	std::cout << BLUE << "#               " << (currentSelection == MenuChoice::Start ? YELLOW : WHITE) << "       START GAME" << BLUE << "                        #\n";
	std::cout << "#               " << (currentSelection == MenuChoice::HighScores ? YELLOW : WHITE) << "       HIGH SCORES" << BLUE << "                       #\n";
	std::cout << "#               " << (currentSelection == MenuChoice::Difficulty ? YELLOW : WHITE) << "       DIFFICULTY" << BLUE << "                        #\n";
	std::cout << "#               " << (currentSelection == MenuChoice::Exit ? YELLOW : WHITE) << "       EXIT" << BLUE << "                              #\n";

	

	std::cout << "#                                                        #\n";
	std::cout << "##########################################################\n";
	std::cout << "#                                                        #\n";
	std::cout << "#                                                        #\n";	
	std::cout << "#                    "<< RED << "   HIGH SCORE" << BLUE << "                       #\n";	
	std::cout << "#                        " << WHITE << mScoretoShow << BLUE << std::string(padding, ' ') << " #\n";//score display needs to be fixed
	std::cout << "#                                                        #\n";
	std::cout << "##########################################################\n";
	std::cout << RESET;
	std::cout << WHITE << "press " << RED << "ESC" << WHITE <<" key at anytime to return to game menu.\n";
	std::cout << WHITE << "Player " << YELLOW << "movement" << WHITE<< ":" << RED << " W, A, S, D " << WHITE << "or" << RED << " Arrow keys" << RESET << "\n";
	std::cout << YELLOW << "F11 " << WHITE << " for FULLSCREEN (Level 2 doesnt show correctly unless you hit F11).";
}



void Menu::setHighScore(int score)
{
	mScoretoShow = score;
}

//For testing ideas before adding to drawMenu
void Menu::draw2()
{
	
	std::cout << "\033[2J\033[1;1H"; //clearscreen
	                                         

	/*std::cout << BLUE << "#" << (currentSelection == MenuChoice::Start ? YELLOW : WHITE) << "[1] START GAME" << BLUE << "#\n";
	std::cout << "#" << (currentSelection == MenuChoice::HighScores ? YELLOW : WHITE) << "[2] HIGH SCORES" << BLUE << "#\n";
	std::cout << "#" << (currentSelection == MenuChoice::Difficulty ? YELLOW : WHITE) << "[3] DIFFICULTY" << BLUE << "#\n";
	std::cout << "#" << (currentSelection == MenuChoice::Exit ? YELLOW : WHITE) << "[4] EXIT" << BLUE << "#\n";*/

	//
	drawLine((currentSelection == MenuChoice::Start) ? " -> START GAME" : " START GAME", (currentSelection == MenuChoice::Start) ? YELLOW : WHITE, BLUE);
	drawLine((currentSelection == MenuChoice::HighScores) ? " -> HIGH SCORES" : " HIGH SCORES", (currentSelection == MenuChoice::HighScores) ? YELLOW : WHITE, BLUE);
	drawLine((currentSelection == MenuChoice::Difficulty) ? " -> Difficulty" : " Difficulty", (currentSelection == MenuChoice::Difficulty) ? YELLOW : WHITE, BLUE);
	drawLine((currentSelection == MenuChoice::Exit) ? " -> Exit" : " Exit", (currentSelection == MenuChoice::Exit) ? YELLOW : WHITE, BLUE);
	
	

	
}
//Get User input
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
			//return currentSelection;
			drawMenu();
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
			drawMenu();
			break;
			//return currentSelection; game was getting stuck because return here and above.
		case 13:
			playerSelection = true;
			break;
		default:
			break;
		}		
	}
	return currentSelection;

};