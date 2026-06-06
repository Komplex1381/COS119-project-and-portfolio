#include "Pacman.h"

Pacman::Pacman(int startRow, int startCol, double speed) : GameCharacter(startRow, startCol, speed, 'C'), mLives(3)
{
}

void Pacman::draw() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<short>(getX()), static_cast<short>(getY()) };
	SetConsoleCursorPosition(hConsole, coord);

	std::cout << YELLOW << static_cast<char>(getTileChar()) << RESET;
}

void Pacman::handleInput()
{
	int currentX = getX();
	int currentY = getY();
	if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) 
	{
		currentY--;
		setY(currentY);
		//sound needs to only play when eatng pellets, update week2.
		SoundManager::playSFX("Eating.wav");
		//setNextDirection(Direction::UP);
	}
	else if ((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) 
	{
		currentY++;
		setY(currentY);
		//setNextDirection(Direction::DOWN);
	}
	else if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) 
	{
		currentX--;
		setX(currentX);
		//setNextDirection(Direction::LEFT);
	}
	else if ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) 
	{
		currentX++;
		setX(currentX);
		//SoundManager::playSFX("Eating.wav");
		//setNextDirection(Direction::RIGHT);
	}
	SoundManager::stop();
}

void Pacman::update(const Map& gameMap)
{
	//int currentX = getX();
	//int currentY = getY();

	int nextX = getX();
	int nextY = getY();
	/*Direction nextDirection = getNextDirection();
	Direction currentDirection = getCurrentDirection();

	switch (nextDirection)
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

	setCurDirection(nextDirection);

	int forwardX = getX();
	int fowardY = getY();

	currentDirection = getCurrentDirection();

	switch (currentDirection)
	{
	case Direction::UP:
		fowardY--;
		break;
	case Direction::DOWN:
		fowardY++;
		break;
	case Direction::LEFT:
		forwardX--;
		break;
	case Direction::RIGHT:
		forwardX++;
		break;
	default:
		break;
	}*/
	
}
