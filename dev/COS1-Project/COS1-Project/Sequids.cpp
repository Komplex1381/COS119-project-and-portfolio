#include "Sequids.h"


Sequids::Sequids(int startRow, int startCol, double speed, WORD color) : Ghost(startRow, startCol, speed, 'S', color), mWaveSpeed(1.0)
{
}

void Sequids::SetWaveSpeed(double speedIncrease)
{
	mWaveSpeed = speedIncrease;

	setSpeed(getSpeed() * mWaveSpeed);
}

double Sequids::getWaveSpeed() const
{
	return mWaveSpeed;
}


