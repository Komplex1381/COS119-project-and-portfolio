#include "GameCharacter.h"

GameCharacter::GameCharacter(int startRow, int startCol, double speed, int tileChar) 
	: mX(startRow), mY(startCol), mSpeed(speed), mCurrentDirection(Direction::NONE), mNextDirection(Direction::NONE), mTileChar(tileChar)
{
}
