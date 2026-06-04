#include "gameManager.h"
#include <Windows.h>
#include <iostream>

GameManager::GameManager() : currentMap(0,0)
{
	mCurrentGameState = GameState::Menu;
	mGameRun = true;
	mCurrentScore = 0;
	mHighScore = 0;
	mCurrentLevel = 1;
	mLevelRun = false;

	loadScores();
	
}

GameManager::~GameManager()
{
	saveSocres();
	for (Highscore* hs : higheScores) 
	{
		delete hs;
	}
	higheScores.clear();
}

void GameManager::run()
{
	hideConsoleCursor();
	while (mGameRun)
	{
		switch (mCurrentGameState)
		{
		case GameState::Menu:
			handleMenuState();
			break;
		case GameState::Start:
			gameplayLoop();
			break;
		case GameState::HighScores:
			handleHighScoreState();
			break;
		case GameState::Difficulty:
			handleDifficultyState();
			break;
		case GameState::Exit:
			mGameRun = false;
			break;
		}
	}
}

void GameManager::loadScores()
{
	higheScores = Highscore::loadHighscoresFromBinary(highscoreFilename);

	if (!higheScores.empty()) 
	{
		std::sort(higheScores.begin(), higheScores.end(), [](const Highscore* a, const Highscore* b)
			{
				return a->score > b->score;
			});
		mHighScore = higheScores[0]->score;
	}
}

void GameManager::saveSocres()
{
	Highscore::saveHighscoresToBinary(highscoreFilename, higheScores);
}

void GameManager::updateHighScore(int finalScore)
{
	if (finalScore > 0) 
	{
		Highscore* newEntry = new Highscore(finalScore, "Pac-Man");
		higheScores.push_back(newEntry);

		std::sort(higheScores.begin(), higheScores.end(), [](const Highscore* a, const Highscore* b)
			{
				return a->score > b->score;
			}); 
		if (higheScores.size() > 5) 
		{
			delete higheScores.back();
			higheScores.pop_back();
		}
		mHighScore = higheScores[0]->score;
		saveSocres();

	}
}

void GameManager::handleMenuState()
{
	gameMenu.drawMenu(mHighScore);

	MenuChoice choice = gameMenu.getUserChoice();

	if (GetKeyState(VK_RETURN) & 0x8000) 
	{
		switch(choice) 
		{
		case MenuChoice::Start:
			mCurrentGameState = GameState::Start;
			break;
		case MenuChoice::HighScores:
			mCurrentGameState = GameState::HighScores;
			break;
		case MenuChoice::Difficulty:
			mCurrentGameState = GameState::Difficulty;
			break;
		case MenuChoice::Exit:
			mCurrentGameState = GameState::Exit;
			break;
		default:
			break;
		}
		//std::cout << "\033[2J\033[1;1H";
		Sleep(200);
	}
}

void GameManager::handleHighScoreState()
{
}

void GameManager::handleDifficultyState()
{
}

void GameManager::initialLevel()
{
	mCurrentScore = 0;
	mLevelRun = true;

	currentMap = MapLoader::loadMap("lvl2.txt");

	//Pacman & ghost logic here
}

void GameManager::gameplayLoop()
{
	initialLevel();
	//std::cout << "\033[2J\033[1;1H"; //clearscreen

	while (mLevelRun) 
	{
		//Player input

		//updateGame();

		//checkCollisions();

		renderGame();

		//Windows call
		Sleep(100);
	}
	cleanLevel();
}

void GameManager::updateGame()
{
	//player & ghost update here
}

void GameManager::renderGame()
{
	setCursorPosition(0,0);

	//Draw HUD Header
	
	//Render Map
	currentMap.renderASCII();

	//Draw player on map

	//Draw HUD Footer
}

void GameManager::checkCollisions()
{
}

void GameManager::cleanLevel()
{
	//Delete all pointers
}

//Windows code for Cursor
void GameManager::setCursorPosition(int x, int y) const
{
	COORD coord = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GameManager::hideConsoleCursor() const
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(out, &cursorInfo);
}
