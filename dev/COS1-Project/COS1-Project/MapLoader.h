#pragma once
#include "Map.h"
#include <iostream>
#include <string>
#include <fstream>

class MapLoader
{
public:
	static Map loadMap(const std::string& filename);
};

