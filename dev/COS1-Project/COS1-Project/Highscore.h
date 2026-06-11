#pragma once
#include <string>
#include <vector>
#include <fstream>


class Highscore
{
private:
public:
	int score;
	std::string name;

	//Default
	Highscore() : score(0), name("") {}

	//Overloaded
	Highscore(int score, const std::string& variable)
		: score(score), name(variable) {
		
	}	
	

	//Save Highscore
	static void saveHighscoresToBinary(const std::string& filename, const std::vector<Highscore*>& highscores)
	{
		std::ofstream fileOut(filename, std::ios_base::binary);
		if (fileOut.is_open())
		{

			int totalScores = highscores.size();
			fileOut.write(reinterpret_cast<char*>(&totalScores), sizeof(totalScores));

			for (const Highscore* hs : highscores)
			{

				if (hs != nullptr)
				{
					int tempScore = hs->score;
					int nameLength = static_cast<int>(hs->name.length());

					fileOut.write(reinterpret_cast<char*>(&tempScore), sizeof(tempScore));
					fileOut.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

					if (nameLength > 0)
					{
						fileOut.write(hs->name.c_str(), nameLength);
					}
				}
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

				if (!fileIn.read(reinterpret_cast<char*>(&tempScore), sizeof(tempScore)) ||
					!fileIn.read(reinterpret_cast<char*>(&tempLength), sizeof(tempLength))) 
				{
					break;
				}
				
								
				Highscore* newScore = new Highscore();
				newScore->score = tempScore;

				if (tempLength > 0)
				{
					//Resize to get rid of garbage input and hold the exact string.
					newScore->name.resize(tempLength);

					fileIn.read(&newScore->name[0], tempLength);
					
				}				

				highscores.push_back(newScore);

			}
			fileIn.close();
		}

		return highscores;
	}

};

