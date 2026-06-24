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
#include <Windows.h>
#include <iostream>
#include "Sequids.h"

enum class GameState { Menu, Start, HighScores, Difficulty, Exit };

class GameManager
{
private:
	GameState mCurrentGameState;
	bool mGameRun;
	int mCurrentScore;
	int mHighScore;
	int mCurrentLevel;
	int mCurrentRound;
	DifficultyChoice mChosenDifficulty;
	bool mLevelRun;
	bool mFullscreen = false;
	Pacman* mPacman = nullptr;
	std::vector<Ghost*> mGhosts;
	std::vector<int> mExitGhostHouse;
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
	int mGhostHouseTimer;
	double mBaseGhostSpeed;

	bool mIsAttacking;
	int mAttackVisualX = -1;
	int mAttackVisualY = -1;
	int mInfinityRayX[3] = { -1 , -1 , -1 };
	int mInfinityRayY[3] = { -1 , -1 , -1 };
	int mActiveRayTiles = 0;


	int mCurrentWave = 1;
	int mGhostsSpawnedInWave = 0;
	int mWaveSpawnTimer = 0;
	double mWaveSpeedModifier = 1.0;
	
	

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
	void playerDeath();
	
	ConsoleWindow* mWindow;

	

public:
	GameManager();
	~GameManager();
	void run();	
	int getHighscore() const;
	void killSequids(int targetX, int TargetY);
};

