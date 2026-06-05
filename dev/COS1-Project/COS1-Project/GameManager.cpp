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

	//loadScores();
	
}

GameManager::~GameManager()
{
	saveScores();
	for (Highscore* hs : highScores) 
	{
		delete hs;
	}
	highScores.clear();
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
			//handleHighScoreState();
			break;
		case GameState::Difficulty:
			//handleDifficultyState();
			break;
		case GameState::Exit:
			mGameRun = false;
			break;
		}
	}
}

int GameManager::getHighscore() const
{
	return mHighScore;
}

void GameManager::loadScores()
{
	//Clear memory first
	for (Highscore* hs : highScores)
	{
		delete hs;
	}
	//clear up highscores
	highScores.clear();

	//Load new scores
	highScores = Highscore::loadHighscoresFromBinary(highscoreFilename);

	if (!highScores.empty()) 
	{
		std::sort(highScores.begin(), highScores.end(), [](const Highscore* a, const Highscore* b)
			{
				return a->score > b->score;
			});
		
		mHighScore = highScores[0]->score;
	}
	else 
	{
		mHighScore = 0;
	}
	gameMenu.setHighScore(mHighScore);
}

void GameManager::saveScores()
{
	Highscore::saveHighscoresToBinary(highscoreFilename, highScores);
}

void GameManager::updateHighScore(int finalScore)
{
	if (finalScore > 0) 
	{
		Highscore* newEntry = new Highscore(finalScore, "Pac-Man");
		highScores.push_back(newEntry);

		std::sort(highScores.begin(), highScores.end(), [](const Highscore* a, const Highscore* b)
			{
				return a->score > b->score;
			}); 
		if (highScores.size() > 5) 
		{
			delete highScores.back();
			highScores.pop_back();
		}
		mHighScore = highScores[0]->score;
		gameMenu.setHighScore(mHighScore);
		saveScores();

	}
}

void GameManager::handleMenuState()
{
	gameMenu.drawMenu();

	MenuChoice choice = gameMenu.getUserChoice();

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
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
	system("cls");
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

	currentMap = MapLoader::loadMap("lvl1.txt");

	fitConsoleToMap(currentMap.getRows(), currentMap.getCols());
	system("cls");
	//std::cout << "\033[2J\033[1;1H";

	//Pacman & ghost logic here
}

void GameManager::gameplayLoop()
{
	initialLevel();
	system("cls");

	while (mLevelRun) 
	{
		//Player input

		//updateGame();

		//checkCollisions();

		renderGame();

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) 
		{
			mLevelRun = false;
			mCurrentGameState = GameState::Menu;
		}

		//Windows call
		Sleep(100);
	}
	system("cls");
	mCurrentGameState = GameState::Menu;
	//cleanLevel();

	
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

//Attempt to make game window map size(more research needed)
void GameManager::fitConsoleToMap(int mapRows, int mapCols)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) 
	{
		return;
	}

	short paddingWidth = 5;
	short paddingHeight = 8;

	short finalHeight = static_cast<short>(mapRows + paddingHeight);
	short finalWidth = static_cast<short>(mapCols + paddingWidth);

	COORD bufferSize = { finalWidth, finalHeight };
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	SMALL_RECT windowSize = { 0,0, static_cast<short>(finalWidth - 1), static_cast<short>(finalHeight - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}
