#pragma once
#include <iostream>
#include "Helper.h"


class Map
{
private:
	int mRows;
	int mCols;	
	std::vector<std::vector<int>> grid;

public:	

	int pacmanX = 0;
	int pacmanY = 0;
	int fruitX = 0;
	int fruitY = 0;	
	std::vector<int> ghostX;
	std::vector<int> ghostY;

	Map(int rows, int cols)	: mRows(rows), mCols(cols), grid(rows, std::vector<int>(cols, 0))
	{
		
	}	

	void setTile(int r, int c, char type) 
	{
		if (r >= 0 && r < mRows && c >= 0 && c < mCols)
		{
			grid[r][c] = type;
		}
	}

	int getRows() const 
	{
		return mRows;
	}

	int getCols() const
	{
		return mCols;
	}

	int getTile(int r, int c) const
	{
		/*if (r < 0 || r >= mRows || c < 0 || c >= mCols)
		{
			return 3;
		}
		return static_cast<char>(grid[r][c]);*/
		if (r < 0 || r >= mRows)
		{
			return 3;
		}
		if (c < 0 || c >= static_cast<int>(grid[r].size()))
		{
			return 3;
		}
		return (grid[r][c]);
		
	}

	bool canMove(int x, int y, bool ghost = false) 
	{
		char tile = getTile(y, x);

		if (tile == 3) 
		{
			return false;
		}
		if (tile == 4) 
		{
			return ghost;
		}

		return (tile != 3);
	}

	bool clearedPellets()const 
	{
		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mCols; j++)
			{
				//check for any pellets or power pellets
				if (grid[i][j] == 1 || grid[i][j] == 2)
				{
					return true;
				}
			}
		}
		return false;
	}

	void renderASCII() const 
	{
		

		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mCols; j++) 
			{
				switch(grid[i][j])
				{
				case 5:
					std::cout << RED << "%";
					break;
				case 4:
					std::cout << WHITE << "-";
					break; 
				case 3:
					std::cout << BLUE << "#";
					break;
				case 2:
					std::cout << YELLOW << "O";
					break;
				case 1:
					std::cout << WHITE << ".";
					break;
				default:
					std::cout << " ";
					break;
				}
			}
			std::cout << "\n";
		}
	}

	/*void renderASCII2(int scale = 1) const
	{
		for (int i = 0; i < mRows; i++)
		{
			for (int vRepeat = 0; vRepeat < scale; vRepeat++) 
			{
				for (int j = 0; j < mCols; j++)
				{
					for(int hRepeat = 0; hRepeat < scale; hRepeat++){
						switch (grid[i][j])
						{
						case 3:
							std::cout << BLUE << "#";
							break;
						case 2:
							std::cout << YELLOW << "O";
							break;
						case 1:
							std::cout << WHITE << ".";
							break;
						default:
							std::cout << " ";
							break;
						}
					}
				}
				std::cout << "\n";
			}
			
		}
	}*/

};

