#pragma once
#include <iostream>
#include "Helper.h"

class Map
{
private:
	int mRows;
	int mCols;
	int** grid = nullptr;

	void mGrid() 
	{
		grid = new int*[mRows];
		for (int i = 0; i < mRows; i++) 
		{
			grid[i] = new int [mCols] {0};
		}
	}

	void mClearGrid() 
	{
		if (grid != nullptr) 
		{
			for (int i = 0; i <  mRows; i++) 
			{
				delete[] grid[i];
			}
			delete[] grid;
			grid = nullptr;
		}
	}

public:	

	Map(int rows, int cols)	: mRows(rows), mCols(cols)
	{
		mGrid();
	}

	~Map() 
	{
		mClearGrid();
	}

	Map(const Map& other) : mRows(other.mRows), mCols(other.mCols) 
	{
		mGrid();
		for (int i = 0; i < mRows; i++) 
		{
			for (int j = 0; j < mCols; j++) 
			{
				grid[i][j] = other.grid[i][j];
			}
		}
	}

	Map& operator=(const Map& other) 
	{
		if (this == &other) 
		{
			return *this;
		}

		mClearGrid();

		mRows = other.mRows;
		mCols = other.mCols;
		mGrid();

		for (int i = 0; i < mRows; i++) 
		{
			for (int j = 0; j < mCols; j++) 
			{
				grid[i][j] = other.grid[i][j];
			}
		}

		return *this;
	}

	void setTile(int r, int c, int type) 
	{
		if (r >= 0 && r < mRows && c >= 0 && c < mCols)
		{
			grid[r][c] = type;
		}
	}

	void renderASCII() const 
	{
		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mCols; j++) 
			{
				switch(grid[i][j])
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
			std::cout << "\n";
		}
	}

};

