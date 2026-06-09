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

	void setCursorPosition(int x, int y) const;
	void hideConsoleCursor() const;
	void fitConsoleToMap(int mapRows, int mapCols);

	void setConsoleFullscreen(bool fullscreen);

public:
	GameManager();
	~GameManager();
	void run();
	int getHighscore() const;
};

