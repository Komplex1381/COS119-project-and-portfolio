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

	int getTileChar() const 
	{
		return mTileChar;
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

