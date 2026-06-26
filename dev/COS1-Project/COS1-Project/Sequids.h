#pragma once
#include "Ghost.h"
#include <iostream>
#include "ConsoleWindow.h"

class Sequids : public Ghost
{
private:
	double mWaveSpeed;
public:
	Sequids(int startRow, int startCol, double speed, WORD color);	
	
	void SetWaveSpeed(double speedIncrease);
	double getWaveSpeed() const;
};

