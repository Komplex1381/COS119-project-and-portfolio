#include "Ghost.h"

Ghost::Ghost(int startRow, int startCol, double speed, int tileChar, int color) : GameCharacter(startRow, startCol, speed, tileChar), mState(GhostState::CHASE), mColor(color)
{
}

void Ghost::draw() const
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<short>(getX()), static_cast<short>(getY()) };
	SetConsoleCursorPosition(hConsole, coord);

	if (mState == GhostState::FRIGHTENED) 
	{
		SetConsoleTextAttribute(hConsole, 9);
	}
	else 
	{
		SetConsoleTextAttribute(hConsole, mColor);
	}

	std::cout << static_cast<char>(getTileChar());
	SetConsoleTextAttribute(hConsole, 7);
}

void Ghost::updateAI(const Map& gameMap, int pacmanX, int pacmanY)
{
	int currentX = getX();
	int currentY = getY();
}
