#pragma once
#include "GameCharacter.h"
#include "Map.h"
#include <windows.h>
#include "SoundManager.h"

class Pacman :  public GameCharacter
{
private:
	int mLives;
	int mPreviousX = 0;
	int mPreviousY = 0;

public:
	Pacman(int startRow, int startCol, double speed);

	void draw() const override;
	void handleInput();
	void update(const Map& gameMap);
	int getPreviousX() const { return mPreviousX; }
	int getPreviousY() const { return mPreviousY; }
	int getLives() const 
	{
		return mLives;
	}
	void increaseLives() 
	{
		mLives++;
	}
	void decreaseLives()
	{
		if (mLives > 0)
		{
			mLives--;
		}
	}
	void setLives(int startingLives)
	{
		mLives = startingLives;
	}

};

