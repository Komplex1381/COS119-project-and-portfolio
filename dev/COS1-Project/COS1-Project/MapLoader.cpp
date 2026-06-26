#include "MapLoader.h"


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
            if (!line.empty() && line.back() == '\r') 
            {
                line.pop_back();
            }
            if (rows == 0) cols = static_cast<int>(line.length());
            rows++;
        }

        fileIn.clear();
        fileIn.seekg(0, std::ios::beg);

        Map newMap(rows, cols);

        int currentRow = 0;
        while (std::getline(fileIn, line) && currentRow < rows) 
        {
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }
            int chars = std::min(static_cast<int>(line.length()), cols);
            for (int col = 0; col < chars; col++) 
            {
                char ch = line[col];

                if (ch == '#') //Walls
                {
                    newMap.setTile(currentRow, col, 3);
                }
                else if (ch == 'O') //Power pellets
                {
                    newMap.setTile(currentRow, col, 2);
                }
                else if (ch == '.') //Pellets
                {
                    newMap.setTile(currentRow, col, 1);
                }
                else if (ch == '-') //ghost door
                {
                    newMap.setTile(currentRow, col, 4);
                }
                else if (ch == ' ')
                {
                    newMap.setTile(currentRow, col, 0);
                }
                else if (ch == 'C') 
                {
                    newMap.pacmanX = col;
                    newMap.pacmanY = currentRow;
                    newMap.setTile(currentRow, col, 0);
                }
                else if (ch == 'G') 
                {
                    newMap.ghostX.push_back(col);
                    newMap.ghostY.push_back(currentRow);
                    newMap.setTile(currentRow, col, 0);
                }
                else if (ch == '%')
                {
                    newMap.fruitX = col;
                    newMap.fruitY = currentRow;
                    newMap.setTile(currentRow, col, 0);
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
