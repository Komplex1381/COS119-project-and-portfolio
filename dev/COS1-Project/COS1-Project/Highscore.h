#pragma once
#include <string>
#include <vector>
#include <fstream>


class Highscore
{
private:
public:
	int score;
	char* name;

	//Default
	Highscore() : score(0), name(nullptr) {}
	//Overloaded
	Highscore(int score, const std::string& variable)
		: score(score) {
		size_t size = variable.length() + 1;
		name = new char[size];
		strcpy_s(name, size, variable.c_str());
	}
	//Copy
	Highscore(const Highscore& other)
		: score(other.score)
	{
		if (other.name != nullptr)
		{
			size_t size = std::strlen(other.name) + 1;
			name = new char[size];
			strcpy_s(name, size, other.name);
		}
		else
		{
			name = nullptr;
		}
	}
	//Copy Assignment Operator for deep copy not shallow
	Highscore& operator=(Highscore other)
	{
		if (this == &other)
		{
			return *this;
		}
		delete[] name;

		this->score = other.score;

		if (other.name != nullptr)
		{
			size_t size = std::strlen(other.name) + 1;
			name = new char[size];
			strcpy_s(name, size, other.name);
		}
		else
		{
			name = nullptr;
		}
		return *this;
	}
	//Destructor
	~Highscore()
	{
		delete[] name;
	}

	//Save Highscore
	static void saveHighscoresToBinary(const std::string& filename, std::vector<Highscore*>& highscores)
	{
		std::ofstream fileOut(filename, std::ios_base::binary);
		if (fileOut.is_open())
		{

			int totalScores = highscores.size();
			fileOut.write(reinterpret_cast<char*>(&totalScores), sizeof(totalScores));

			for (const Highscore* hs : highscores)
			{

				int tempScore = hs->score;
				int nameLength = strlen(hs->name);

				fileOut.write(reinterpret_cast<char*>(&tempScore), sizeof(tempScore));
				fileOut.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
				fileOut.write(hs->name, nameLength);
			}
			fileOut.close();
		}
	}

	//Load Highscore
	static std::vector<Highscore*> loadHighscoresFromBinary(const std::string& filename)
	{
		std::vector<Highscore*> highscores;
		std::ifstream fileIn(filename, std::ios_base::binary);

		if (fileIn.is_open())
		{
			int totalScores = 0;
			fileIn.read(reinterpret_cast<char*>(&totalScores), sizeof(totalScores));

			highscores.reserve(totalScores);

			for (int i = 0; i < totalScores; i++)
			{
				int tempScore = 0;
				int tempLength = 0;

				fileIn.read(reinterpret_cast<char*>(&tempScore), sizeof(tempScore));
				fileIn.read(reinterpret_cast<char*>(&tempLength), sizeof(tempLength));


				Highscore* newScore = new Highscore();
				newScore->score = tempScore;

				newScore->name = new char[tempLength + 1];

				fileIn.read(newScore->name, tempLength);

				newScore->name[tempLength] = '\0';

				highscores.push_back(newScore);

			}
			fileIn.close();
		}

		return highscores;
	}

};

