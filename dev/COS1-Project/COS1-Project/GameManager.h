#pragma once
#include"Menu.h"
#include "MenuChoice.h"
#include "Map.h"
#include "MapLoader.h"
#include "Highscore.h"
#include "Pellet.h"
#include "Item.h"
#include "Fruit.h"
#include "Helper.h"
#include "Pacman.h"
#include "Ghost.h"
#include "SoundManager.h"
#include "ConsoleWindow.h"

enum class GameState { Menu, Start, HighScores, Difficulty, Exit };

class GameManager
{
private:
	GameState mCurrentGameState;
	bool mGameRun;
	int mCurrentScore;
	int mHighScore;
	int mCurrentLevel;
	bool mLevelRun;
	bool mFullscreen = false;
	Pacman* mPacman = nullptr;
	std::vector<Ghost*> mGhosts;
	//Fruit Timer and Show
	int mFruitTimer;
	bool mShowFruit;

	const int mSpawnFruit = 100;
	const int mFruitVanish = 100;
	//Power Pellet
	bool mPowerPellet;
	int mPowerPelletTimer;

	const int mChase = 200;
	const int mScatter = 80;
	const int mFrightened = 80;
	GhostState mGlobalGhostState = GhostState::SCATTER;

	int mGhostStateTimer;
	

	Menu gameMenu;
	Map currentMap;

	std::vector<Highscore*> highScores;
	const std::string highscoreFilename = "highscores.bin";

	void loadScores();
	void saveScores();
	void updateHighScore(int finalScore);

	void handleMenuState();
	void handleHighScoreState();
	void handleDifficultyState();

	void initialLevel();
	void gameplayLoop();
	void updateGame();
	void renderGame();
	void checkCollisions();
	void cleanLevel();
	
	ConsoleWindow* mWindow;

	

public:
	GameManager();
	~GameManager();
	void run();
	int getHighscore() const;
};

