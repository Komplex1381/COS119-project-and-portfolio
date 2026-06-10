#pragma once
#include "Item.h"
#include <string>
#include <iostream>

class Fruit :  public Item
{
private:
	std::string mFruitName;
public:

	//constructor, which fruit and value (Add struct or enum for fruit and values)
	Fruit(const std::string& name, int points) : Item(points), mFruitName(name)
	{
	}

	//Fruit eaten
	void eaten() override 
	{
		std::cout << Item::getPoints(); //not sure if this works the way I want yet.
	}
};

