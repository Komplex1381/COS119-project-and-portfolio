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

	Menu gameMenu;
	Map currentMap;

	std::vector<Highscore*> higheScores;
	const std::string highscoreFilename = "highscores.bin";

	void loadScores();
	void saveSocres();
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

public:
	GameManager();
	~GameManager();
	void run();
};

