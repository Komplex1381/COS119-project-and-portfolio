#include "MapLoader.h"
#include <iostream>
#include <string>
#include <fstream>

//MapLoader class so that later I can upgrade graphics to SFML or SDL.
Map MapLoader::loadMap(const std::string& filename)
{
    std::ifstream fileIn(filename);
    if (fileIn.is_open()) 
    {
        int rows = 0;
        int cols = 0;
        std::string line;

        while (std::getline(fileIn, line))
        {
            if (rows == 0) cols = line.length();
            rows++;
        }

        fileIn.clear();
        fileIn.seekg(0, std::ios::beg);

        Map newMap(rows, cols);

        int currentRow = 0;
        while (std::getline(fileIn, line) && currentRow < rows) 
        {
            int chars = std::min(static_cast<int>(line.length()), cols);
            for (int col = 0; col < chars; col++) 
            {
                char ch = line[col];

                if (ch == '#') 
                {
                    newMap.setTile(currentRow, col, 3);
                }
                else if (ch == 'O') 
                {
                    newMap.setTile(currentRow, col, 2);
                }
                else if (ch == '.') 
                {
                    newMap.setTile(currentRow, col, 1);
                }
                else 
                {
                    newMap.setTile(currentRow, col, 0);
                }
            }
            currentRow++;
        }
        fileIn.close();
        return newMap;
    }

    return Map(0, 0);

    
}
