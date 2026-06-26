#pragma once
#include <iostream>
#include <string>
#include <random>
//Change to enum later
inline const std::string RED = "\033[31m";
inline const std::string GREEN = "\033[32m";
inline const std::string YELLOW = "\033[33m";
inline const std::string BLUE = "\033[34m";
inline const std::string CYAN = "\033[36m";
inline const std::string WHITE = "\033[97m";
inline const std::string BROWN = "\033[38;5;94m";
inline const std::string RESET = "\033[0m";
//"\033[38;5;94mThis text is brown\033[0m\n"

namespace Helper
{


static bool IsInteger(const char* number) 
{
	//int convertedNumber = std::stoi(number); std::from_chars
	if (number == nullptr || number[0] == '\0')
	{
		std::cout << "Invalid input.";
		return false;
	}
	try 
	{
		std::size_t check = 0;
		int convertedNumber = std::stoi(number, &check);
		
		if (number[check] != '\0')
		{
			std::cout << "conversion failed.\n";
			return false;
		}

		std::cout << number;
		return true;
	}	
	catch (const std::invalid_argument&)
	{
		std::cout << "conversion failed.\n";
	}
	catch (const std::out_of_range&)
	{
		std::cout << "conversion failed.\n";
	}
	catch (const std::exception&)
	{
		std::cout << "conversion failed.\n";
	}
	
}

static void PrintIntegerBinary(int * number) 
{
	//Bitset NOT Allowed
	//Must print all 32 bits
	for (int i = 31; i >= 0; i--) 
	{
		int bits = (*number >> i) & 1;
		printf("%d", bits);
	}
	printf("\n");
}

static void BubbleSort(int* array, int size) //const
{
	
	int n = size;
	bool swapped;
	do
	{
		
		swapped = false;
		
		for (int i = 1; i <= n - 1; i++)
		{
			
			if (array[i - 1] > array[i])
			{
				
				std::swap(array[i - 1], array[i]);
				
				swapped = true;
			}
		}
		n = n - 1;
	} while (swapped);

}

static void ClearInputBuffer() 
{
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

static void PauseConsoleWindow() 
{
	std::cout << "Pess Enter to continue ...";
	std::cin.get();
}

static void Border()
{
	std::cout << "===================================\n";
	
}
static void TitleC(const std::string& title, const std::string& borderColor, const std::string& textColor)
{
	
	std::cout << borderColor << "===================================\n";
	std::cout << "===\t    " << textColor << title << borderColor << "    \t===\n";
	std::cout << "===================================\n\n" << RESET;

}
static void Title(const std::string& title)
{
	std::cout << "===================================\n";
	std::cout << "===\t" << title << "\t===\n";
	std::cout << "===================================\n\n";

}
static void TestOutput(const std::string& testName)
{
	std::cout << testName;	

}
static int ValidateInteger(const std::string& input)
{
	int selection;
	while (true)
	{
		std::cout << input;
		if (std::cin >> selection)
		{
			return selection;
		}
		std::cout << "Invalid input. Please enter a valid number.\n";
		ClearInputBuffer();
	}
}

static void GetStringInput(const std::string& input, std::string& output)
{
	std::cout << input;
	std::getline(std::cin, output);
}
static int Random4Digit()
{
	return (std::rand() % 9000) + 1000;
}

static bool CoinFlip()
{
	//Random CoinFlip example code
	//std::random_device rd;  // Obtain a seed from the system
	//std::mt19937 gen(rd()); // Seed the generator
	//std::uniform_int_distribution<> distrib(0, 1); // for the bool true or false like heads or tails
	//int random_number = distrib(gen); 
	// return distrib(gen) == 1; // Generate the number
	return (rand() % 2) == 1;
	
}

static int MenuChoice(const std::string& input) 
{
	int selection = 0;
	std::cout << input;

	while (!(std::cin >> selection))
	{
		
		ClearInputBuffer();
		std::cout << "Invalid input. Please enter a valid ID\n";
			
	}
	return selection;
}

}
