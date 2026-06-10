#pragma once
#include "Item.h"
#include <iostream>

class Pellet :  public Item
{
public:

	//constructor, give pellet its value
	Pellet() : Item(10) 
	{
	}

	void eaten() override 
	{
		std::cout << "+10pts\n";
	}
};

