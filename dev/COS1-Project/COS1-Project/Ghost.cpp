#include "Ghost.h"

Ghost::Ghost(int startRow, int startCol, double speed, int tileChar, int color) : GameCharacter(startRow, startCol, speed, tileChar), mState(GhostState::CHASE), mColor(color)
{
}

double calculateDistance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return std::sqrt((dx * dx) + (dy * dy));
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

	int targetX = pacmanX;
	int targetY = pacmanY;

	if (mState == GhostState::SCATTER) 
	{
		targetX = (mColor % 2 == 0) ? 0 : gameMap.getCols() - 1;
		targetY = (mColor > 12) ? 0 : gameMap.getRows() - 1;
	}
	else if (mState == GhostState::EATEN) 
	{
		targetX = gameMap.getCols() / 2;
		targetY = gameMap.getRows() / 2;

		if (currentX == targetX && currentY == targetY) 
		{
			mState = GhostState::CHASE;
		}
	}

	Direction choices[] = { Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT };
	Direction bestDirection = Direction::NONE;
	double shortestDistance = (std::numeric_limits<double>::max)();

	std::vector<Direction> validChoice;

	for (Direction direction : choices) 
	{
		if ((direction == Direction::UP && getCurrentDirection() == Direction::DOWN) || 
			(direction == Direction::DOWN && getCurrentDirection() == Direction::UP) || 
			(direction == Direction::LEFT && getCurrentDirection() == Direction::RIGHT) ||
			(direction == Direction::RIGHT && getCurrentDirection() == Direction::LEFT))
		{
			continue;
		}


		int checkX = currentX;
		int checkY = currentY;

		if (direction == Direction::UP) 
		{ checkY--; }
		if (direction == Direction::DOWN) 
		{ checkY++; }
		if (direction == Direction::LEFT) 
		{ checkX--; }
		if (direction == Direction::RIGHT) 
		{ checkX++; }

		//Tunnels
		if (checkX < 0)
		{
			checkX = gameMap.getCols() - 1;
		}
		else if(checkX >= gameMap.getCols())
		{
			checkX = 0;
		}

		//up and down boundries
		if (checkY < 0)
		{
			checkY = gameMap.getRows() - 1;
			
		}
		else if (checkY >= gameMap.getCols())
		{
			checkY = 0;
		}

		//Wall restriction
		if (gameMap.getTile(checkY, checkX) == 3) 
		{
			continue;
		}

		//Running
		if (mState == GhostState::FRIGHTENED) 
		{
			validChoice.push_back(direction);
			continue;
		}

		double distance = calculateDistance(checkX, checkY, targetX, targetY);

		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			bestDirection = direction;

		}
	}
	if (mState == GhostState::FRIGHTENED) 
	{
		if (!validChoice.empty()) 
		{
			int random = std::rand() % validChoice.size();
			bestDirection = validChoice[random];
		}
	}
	if (bestDirection != Direction::NONE) 
	{
		setCurDirection(bestDirection);

		if (bestDirection == Direction::UP)
		{
			currentY--;
		}
		if (bestDirection == Direction::DOWN)
		{
			currentY++;
		}
		if (bestDirection == Direction::LEFT)
		{
			currentX--;
		}
		if (bestDirection == Direction::RIGHT)
		{
			currentX++;
		}

		if (currentX < 0) 
		{
			currentX = gameMap.getCols() - 1;
		}
		else if (currentX >= gameMap.getCols()) 
		{
			currentX = 0;

			
		}
		setX(currentX);
		setY(currentY);

	}

}
