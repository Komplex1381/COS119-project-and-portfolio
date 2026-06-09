#include "Pacman.h"

Pacman::Pacman(int startRow, int startCol, double speed) : GameCharacter(startRow, startCol, speed, 'C'), mLives(3)
{
}

void Pacman::draw() const
{
	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<short>(getX()), static_cast<short>(getY()) };
	SetConsoleCursorPosition(hConsole, coord);*/

	std::cout << YELLOW << static_cast<char>(getTileChar()) << RESET;
}

void Pacman::handleInput()
{
	
	if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) 
	{
		setNextDirection(Direction::UP);
	}
	else if ((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) 
	{
		setNextDirection(Direction::DOWN);
	}
	else if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) 
	{
		setNextDirection(Direction::LEFT);
	}
	else if ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) 
	{
		setNextDirection(Direction::RIGHT);
	}
	
}

void Pacman::update(const Map& gameMap)
{
	int currentX = getX();
	int currentY = getY();

	int nextX = currentX;
	int nextY = currentY;
	

	switch (getNextDirection())
	{
	case Direction::UP:
		nextY--;
		break;
	case Direction::DOWN:
		nextY++;
		break;
	case Direction::LEFT:
		nextX--;
		break;
	case Direction::RIGHT:
		nextX++;
		break;
	default:
		break;
	}
	//Tunnel
	if (nextX < 0) 
	{
		nextX = gameMap.getCols() - 1;
	}
	else if (nextX >= gameMap.getCols())
	{
		nextX = 0;
	}
	
	char nextTile = gameMap.getTile(nextY, nextX);

	if (getNextDirection() != Direction::NONE && nextTile != 3)
	{
		setX(nextX);
		setY(nextY);

		setCurDirection(getNextDirection());
		return;
	}
	
	nextX = currentX;
	nextY = currentY;

	switch (getCurrentDirection())
	{
	case Direction::UP:
		nextY--;
		break;
	case Direction::DOWN:
		nextY++;
		break;
	case Direction::LEFT:
		nextX--;
		break;
	case Direction::RIGHT:
		nextX++;
		break;
	default:
		break;
	}

	if (nextX < 0)
	{
		nextX = gameMap.getCols() - 1;
	}
	else if (nextX >= gameMap.getCols())
	{
		nextX = 0;
	}

	nextTile = gameMap.getTile(nextY, nextX);

	if (getNextDirection() != Direction::NONE && nextTile != 3)
	{
		setX(nextX);
		setY(nextY);		
	}
	else
	{
		setCurDirection(Direction::NONE);
	}
	
}
