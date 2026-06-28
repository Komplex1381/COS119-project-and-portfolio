#include "Ghost.h"

Ghost::Ghost(int startRow, int startCol, double speed, int tileChar, WORD color) : GameCharacter(startRow, startCol, speed, tileChar), mState(GhostState::CHASE), mColor(color)
{
}

double calculateDistance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return std::sqrt((dx * dx) + (dy * dy));
}

WORD Ghost::getColor() const
{
	return mColor;
}

void Ghost::draw() const
{

	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
	SetConsoleTextAttribute(hConsole, 7);*/
}

void Ghost::updateAI(const Map& gameMap, int pacmanX, int pacmanY, Direction pacmanDirection, int blinkyX, int blinkyY)
{
	int currentX = getX();
	int currentY = getY();

	mPreviousX = currentX;
	mPreviousY = currentY;

	int targetX = pacmanX;
	int targetY = pacmanY;

	if (mState == GhostState::SCATTER) 
	{
		if (mColor ==(FOREGROUND_RED | FOREGROUND_INTENSITY)) //BLINKY - RED GHOST TARGET TOP RIGHT OF MAP
		{
			targetX = gameMap.getCols() - 1;
			targetY = 0;
		}
		else if (mColor == (FOREGROUND_RED | FOREGROUND_BLUE |FOREGROUND_INTENSITY))//PINKY - PINK GHOST TARGET TOP LEFT OF MAP
		{
			targetX = 0;
			targetY = 0;
		}
		else if (mColor == (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)) //INKY - CYAN GHOST TARGET BOTTOM RIGHT OF MAP
		{
			targetX = gameMap.getCols() - 1;
			targetY = gameMap.getRows() - 1;
		}
		else //CLYDE - ORANGE GHOST TARGET BOTTOM LEFT OF MAP
		{
			targetX = 0;
			targetY = gameMap.getRows() - 1;
		}
		
	}//Make ghost have distinct chase personalities
	else if (mState == GhostState::CHASE)
	{
		if (mColor == (FOREGROUND_RED | FOREGROUND_INTENSITY)) //BLINKY - RED GHOST TARGET PACMAN
		{
			targetX = pacmanX;
			targetY = pacmanY;
		}
		else if (mColor == (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY))//PINKY - PINK GHOST TRY TO AMBUSH PACMAN
		{
			targetX = pacmanX;
			targetY = pacmanY;
			if (pacmanDirection == Direction::UP) 
			{
				targetY -= 4; 
				targetX -= 4;
			}
			if (pacmanDirection == Direction::DOWN) 
			{ 
				targetY += 4;
			}
			if (pacmanDirection == Direction::LEFT) 
			{
				targetX -= 4;
			}
			if (pacmanDirection == Direction::RIGHT) 
			{
				targetX += 4;
			}
		}
		else if (mColor == (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)) //INKY - CYAN GHOST ASSIST
		{
			int pacX = pacmanX;
			int pacY = pacmanY;

			if (pacmanDirection == Direction::UP) { pacY -= 2;  }
			if (pacmanDirection == Direction::DOWN) { pacY += 2; }
			if (pacmanDirection == Direction::LEFT) { pacX -= 2; }
			if (pacmanDirection == Direction::RIGHT) { pacX += 2; }

			targetX = blinkyX + 2 * (pacX - blinkyX);
			targetY = blinkyY + 2 * (pacY - blinkyY);
		}
		else //CLYDE - ORANGE GHOST CHASE FROM THE REAR
		{
			double distanceToPacman = calculateDistance(currentX, currentY, pacmanX, pacmanY);
			if (distanceToPacman >= 8.0)
			{ 
				targetX = pacmanX;
				targetY = pacmanY;
			}
			else 
			{
				targetX = 0;
				targetY = gameMap.getRows() - 1;
			}
			
		}
	}
	else if (mState == GhostState::EATEN) 
	{
		targetX = 13; // gameMap.getCols() / 2;
		targetY = 10; // gameMap.getRows() / 2 + 1;

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
		//NO REVERSING
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
		else if (checkY >= gameMap.getRows())
		{
			checkY = 0;
		}
		char tileCheck = gameMap.getTile(checkY, checkX);
		//Wall restriction
		if (tileCheck == 3)
		{
			continue;
		}

		if (tileCheck == 4)
		{
			if (mState == GhostState::EATEN) 
			{
				//let ghost pass 
			}
			else if (direction == Direction::UP) //&& checkY < getY()
			{
				//Let them to escape the house
			}
			else { continue; }
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
	}//Movement choice
	if (mState == GhostState::FRIGHTENED) 
	{
		if (!validChoice.empty()) 
		{
			int random = std::rand() % validChoice.size();
			bestDirection = validChoice[random];
		}
	}//Update positions
	if (bestDirection != Direction::NONE) 
	{
		setCurDirection(bestDirection);
		//int checkX = currentX;

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

		if (currentY < 0)
		{
			currentY = gameMap.getRows() - 1;
		}
		else if (currentY >= gameMap.getRows())
		{
			currentY = 0;


		}
		setX(currentX);
		setY(currentY);

	}

}
