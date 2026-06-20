#pragma once

enum class Direction 
{
	NONE, 
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class GameCharacter
{
private:
	int mX;
	int mY;
	double mSpeed;
	double mMovementCounter = 0.0;
	Direction mCurrentDirection;
	Direction mNextDirection;
	int mTileChar;

public:

	
	GameCharacter(int startRow, int startCol, double speed, int tileChar);

	virtual ~GameCharacter() 
	{
	}

	virtual void draw() const = 0;

	int getX() const 
	{
		return mX;
	}

	int getY() const
	{
		return mY;
	}

	void setX(int x)
	{
		mX = x;
	}

	void setY(int y)
	{
		mY = y;
	}
	void setTile(char newTile) 
	{
		mTileChar = newTile;
	}
	int getTileChar() const 
	{
		return mTileChar;
	}
	double getSpeed() const
	{
		return mSpeed;
	}
	void setSpeed(double speed) 
	{
		mSpeed = speed;
	}
	void increaseMovement() 
	{
		mMovementCounter += mSpeed;
	}

	bool canMove() 
	{
		return mMovementCounter >= 0.6;
	}

	void decreaseMovement() 
	{
		mMovementCounter -= 0.6;
	}

	Direction getCurrentDirection() const 
	{
		return mCurrentDirection;
	}

	Direction getNextDirection() const
	{
		return mNextDirection;
	}

	void setNextDirection(Direction direction)
	{
		mNextDirection = direction;
	}

	void setCurDirection(Direction direction)
	{
		mCurrentDirection = direction;
	}
};

