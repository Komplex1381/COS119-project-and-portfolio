#pragma once
class Item
{
private:
	int mScore;
public:

	Item(int score)	: mScore(score)
	{
	}
	
	virtual ~Item() 
	{
	}

	virtual void eaten() = 0;

	int getPoints() const 
	{
		return mScore;
	}
};

