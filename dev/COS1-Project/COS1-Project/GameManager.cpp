#include "gameManager.h"
#include <Windows.h>
#include <iostream>


GameManager::GameManager() : currentMap(0,0)
{
	ConsoleWindow::initialize(L"PAC-MAN P&P Edition", 80, 80);

	mWindow = new ConsoleWindow(80, 80);

	mCurrentGameState = GameState::Menu;
	mGameRun = true;
	mCurrentScore = 0;
	mHighScore = 0;
	mCurrentLevel = 1;
	mLevelRun = false;

	ConsoleWindow::hideConsoleCursor();
	loadScores();
	gameMenu.setHighScore(mHighScore);
	
}

GameManager::~GameManager()
{
	cleanLevel();

	saveScores();

	delete mWindow;
	
	for (Highscore* hs : highScores) 
	{
		delete hs;
	}
	highScores.clear();
}

void GameManager::run()
{
	ConsoleWindow::hideConsoleCursor();
	while (mGameRun)
	{
		switch (mCurrentGameState)
		{
		case GameState::Menu:
			handleMenuState();
			break;
		case GameState::Start:
			initialLevel();
			gameplayLoop();
			break;
		case GameState::HighScores:
			handleHighScoreState();
			break;
		case GameState::Difficulty:
			handleDifficultyState();
			break;
		case GameState::Exit:
			saveScores();
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
			saveScores();
			mCurrentGameState = GameState::Exit;
			break;
		default:
			break;
		}
		//pause game allow for input
		Sleep(200);
	}
	system("cls");
}

void GameManager::handleHighScoreState()
{
	system("cls");
	Helper::TitleC("HIGH SCORE", WHITE, RED);
	for (int i = 0; i < highScores.size(); i++) 
	{
		std::cout << i + 1 << ") " << highScores[i]->name << " " << highScores[i]->score << "\n";
	}
	std::cout << "Press ESC to return to Menu...";
	while (mCurrentGameState == GameState::HighScores)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			mCurrentGameState = GameState::Menu;
		}
		Sleep(30);
	}
	//
	system("cls");
	Sleep(10);
	
}

void GameManager::handleDifficultyState()
{
	system("cls");
	Helper::TitleC("DIFFICULTY", WHITE, YELLOW);
	std::cout << BLUE << "Please select Difficulty: \n" << RESET;
	//This will change later once I get game running and add feature.
	std::cout << "Easy\n";
	std::cout << "Normal\n";
	std::cout << "Hard\n\n";

	std::cout << "Press ESC to return to Menu...";
	while (mCurrentGameState == GameState::Difficulty)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			mCurrentGameState = GameState::Menu;
		}
	}
	Sleep(10);
	
}

void GameManager::initialLevel()
{
	ConsoleWindow::initialize(L"PAC-MAN P&P Edition", 80, 80);
	mCurrentScore = 0;
	mLevelRun = true;
	mFruitTimer = 0;
	//load map
	currentMap = MapLoader::loadMap("lvl2.txt");

	ConsoleWindow::fitConsoleToMap(currentMap.getRows(), currentMap.getCols());
	system("cls");
	//currentMap.renderASCII();

	//Pacman & ghost logic here
	mPacman = new Pacman(currentMap.pacmanX, currentMap.pacmanY, 0.5);

	WORD ghostColors[4] = 
	{ 
		FOREGROUND_RED | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
	};

	/*int colors[] = { 12,11,13,14 };
	for (int i = 0; i < currentMap.ghostX.size() && i < 4; i++) 
	{
		mGhosts.push_back(new Ghost(currentMap.ghostX[i], currentMap.ghostY[i], 1.0, 'G', colors[i]));
	}*/
	
	currentMap.renderASCII();
	if (mPacman != nullptr)
	{
		ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());
		mPacman->draw();
	}

	//Draw Ghost
	for(size_t i  = 0; i < currentMap.ghostX.size() && i < 4; i++) 
	{
		mGhosts.push_back(new Ghost(currentMap.ghostX[i], currentMap.ghostY[i], 1.0, 'G', ghostColors[i]));
	}
	/*for (const Ghost* ghost : mGhosts)
	{
		if (ghost != nullptr)
		{
			ConsoleWindow::setCursorPosition(ghost->getX(), ghost->getY());
			ghost->draw();
		}
	}*/

	SoundManager::playSFX("StartMusic.wav");

	Sleep(4200);

}

void GameManager::gameplayLoop()
{
	//initialLevel();
	system("cls");
	//currentMap.renderASCII();

	mPowerPellet = false;
	mPowerPelletTimer = 0;
	mGhostStateTimer = 0;

	while (mLevelRun) 
	{
		//Fruit timer
		mFruitTimer++;		
		if (!mShowFruit && mFruitTimer >= mSpawnFruit) 
		{
			currentMap.setTile(currentMap.fruitY, currentMap.fruitX, 5);
			mShowFruit = true;
			mFruitTimer = 0;

			ConsoleWindow::setCursorPosition(currentMap.fruitX, currentMap.fruitY);
			std::cout << RED << "%" << RESET;
		}
		else if (mShowFruit && mFruitTimer >= mFruitVanish) 
		{
			if (currentMap.getTile(currentMap.fruitY, currentMap.fruitX) ==5) 
			{
				currentMap.setTile(currentMap.fruitY, currentMap.fruitX, 0);
				ConsoleWindow::setCursorPosition(currentMap.fruitX, currentMap.fruitY);
				std::cout << " ";
			}
			mShowFruit = false;
			mFruitTimer = 0;
		}
		if (mPowerPellet) 
		{
			mPowerPelletTimer--;
			if (mPowerPelletTimer <= 0) 
			{
				mPowerPellet = false;
				for (Ghost* ghost : mGhosts) 
				{
					//GhostState::CHASE;
					if (ghost != nullptr) //ghost != nullptr && ghost.getState() == GhostState::FRIGHTENED) 
					{
						ghost->setState(GhostState::CHASE);
					}
				}
			}
		}

		//Player input
		mPacman->handleInput();
		updateGame();

		if (GetAsyncKeyState(VK_F11) & 0x8000)
		{
			//flip variable
			mFullscreen = !mFullscreen;

			ConsoleWindow::setConsoleFullscreen(mFullscreen);

			if (!mFullscreen) 
			{
				ConsoleWindow::fitConsoleToMap(currentMap.getRows(), currentMap.getCols());
			}
			system("cls");
			Sleep(250);
		}
		checkCollisions();
		

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) 
		{
			mLevelRun = false;
			mCurrentGameState = GameState::Menu;
		}
		renderGame();
		
		Sleep(100);
	}
	system("cls");
	mCurrentGameState = GameState::Menu;
	updateHighScore(mCurrentScore);
	cleanLevel();

	
}

void GameManager::updateGame()
{
	//player & ghost update here
	if (mPacman != nullptr)
	{
		ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());
		std::cout << " ";
	}

	//Draw Ghost
	for (const Ghost* ghost : mGhosts)
	{
		if (ghost != nullptr)
		{
			ConsoleWindow::setCursorPosition(ghost->getX(), ghost->getY());
			char tileToShow = currentMap.getTile(ghost->getY(), ghost->getX());
			if (tileToShow == 1) 
			{
				std::cout << WHITE << ".";
			}
			else if (tileToShow == 2) 
			{
				std::cout << YELLOW << "O";
			}
			else if (tileToShow == 4)
			{
				std::cout << WHITE << "-";
			}
			else if (tileToShow == 5)
			{
				std::cout << RED << "%";
			}
			else 
			{
				std::cout << " ";
			}
		}
	}

	if (mPacman != nullptr)
	{
		mPacman->update(currentMap);
	}

	for (Ghost* ghost : mGhosts)
	{
		if (ghost != nullptr && mPacman != nullptr) 
		{
			ghost->updateAI(currentMap, mPacman->getX(), mPacman->getY());
		}
	}
}

void GameManager::renderGame()
{
	mWindow->ClearBuffer();

	int mapRows = currentMap.getRows();
	int mapCols = currentMap.getCols();
	WORD white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	WORD yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

	//ConsoleWindow::setCursorPosition(0,0);

	//Draw HUD Header
	std::string score = "Score: " + std::to_string(mCurrentScore);
	for (size_t i = 0; i < score.length(); i++) 
	{
		mWindow->Draw(i, 0, score[i], yellow);
	}
	//std::cout << WHITE << "Score: " << YELLOW << mCurrentScore;
	//setCursorPosition(mapCols / 2, 0);
	std::string hiScore = "HIGH SCORE: " + std::to_string(mHighScore);
	int hiScoreX = mapCols - static_cast<int>(hiScore.length());
	/*if (hiScoreX > 0) 
	{
		for (size_t i = 0; i < hiScore.length(); i++)
		{
			mWindow->Draw(i= hiScoreX + static_cast<int>(i),0, hiScore[i], FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
	}*/
	//std::cout << WHITE << "HIGH SCORE: " << RED << mHighScore << "\n";
	//std::cout << BLUE << std::string(mapCols, '=') << "\n" << RESET;
	//Render Map
	//setCursorPosition(0,2);
	//currentMap.renderASCII();
	for (int y = 0; y < mapRows; y++) 
	{
		for (int x = 0; x < mapCols; x++)
		{
			char tile = currentMap.getTile(y, x);
			int renderY = y; 

			if (tile == 1)
			{
				//std::cout << WHITE << ".";
				mWindow->Draw(x, renderY, '.', white);
			}
			else if (tile == 2)
			{
				//std::cout << YELLOW << "O";
				mWindow->Draw(x, renderY, 'O', yellow);
			}
			else if (tile == 3)
			{
				//std::cout << YELLOW << "#";
				mWindow->Draw(x, renderY, '#', FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else if (tile == 4)
			{
				//std::cout << WHITE << "-";
				mWindow->Draw(x, renderY, '-', white);
			}
			else if (tile == 5)
			{
				//std::cout << RED << "%";
				mWindow->Draw(x, renderY, '%', FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			
		}
	}

	//Draw player on map
	if (mPacman != nullptr) 
	{
		//ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());

		mWindow->Draw(mPacman->getX(), mPacman->getY(), 'C', yellow);
	}

	//Draw Ghost
	for (const Ghost* ghost : mGhosts) 
	{
		if (ghost != nullptr) 
		{
			WORD ghostColor;;
			if (mPowerPellet) 
			{
				ghostColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			}
			else 
			{
				ghostColor = ghost->getColor();
			}
			//ConsoleWindow::setCursorPosition(ghost->getX(), ghost->getY());
			mWindow->Draw(ghost->getX(), ghost->getY(), 'G', ghostColor);
		}
	}
	mWindow->Display();

	//Draw HUD Footer
	//int BottumHud = mapRows;
	//ConsoleWindow::setCursorPosition(0, BottumHud);
	////std::cout << BLUE << std::string(mapCols, '=') << "\n" << RESET;
	////setCursorPosition(0, BottumHud + 1);
	//std::cout << WHITE << "LIVES: ";
	//
	//int totalLives = (mPacman != nullptr) ? mPacman->getLives() : 0;
	//for (int i = 0; i < totalLives; i++) 
	//{
	//	std::cout << YELLOW << "C " << RESET;
	//}

	//std::cout << WHITE << "           ";
	//ConsoleWindow::setCursorPosition(mapCols - 15, BottumHud);
	//std::cout << YELLOW << "FRUIT: " << RED << "%" << RESET << "    \n";

}


void GameManager::checkCollisions()
{
	Pellet pellet;

	if (mPacman == nullptr) 
	{
		return;
	}
	int pacX = mPacman->getX();
	int pacY = mPacman->getY();
	char currentTile = currentMap.getTile(pacY,pacX);

	//Pellet munch
	if (currentTile == 1)
	{
		currentMap.setTile(pacY, pacX, 0);
		mCurrentScore += 10;
		//Needs to display @ pacman x and y
		/*ConsoleWindow::setCursorPosition(pacX, pacY);
		pellet.eaten();*/

		//SoundManager::playSFX("Eating.wav");
	}
	else if (currentTile == 2) //POWER Pellet
	{
		currentMap.setTile(pacY, pacX, 0);
		mCurrentScore += 50;

		mPowerPellet = true;
		mPowerPelletTimer = 50;

		for (Ghost* ghost : mGhosts)
		{
			if (ghost != nullptr)
			{
				ghost->setState(GhostState::FRIGHTENED);
			}
		}
	}
	else if (currentTile == 5)
	{
		Fruit cherry("Cherry", 100);
		ConsoleWindow::setCursorPosition(pacX, pacY);
		cherry.eaten();

		mCurrentScore += cherry.getPoints();
		currentMap.setTile(pacY, pacX, 0);

		mShowFruit = false;
		mFruitTimer = 0;
	}

	//Ghost Collision
	for (int i = 0; i < mGhosts.size(); i++)
	{
		Ghost* ghost = mGhosts[i];
		if (ghost == nullptr)
		{
			continue;
		}
			int ghostX = ghost->getX();
			int ghostY = ghost->getY();

			if (pacX == ghost->getX() && pacY == ghost->getY())
			{
				if (mPowerPellet && ghost->getState() == GhostState::FRIGHTENED) 
				{
					mCurrentScore += 200;
					//Sound effect
					SoundManager::playSFX("GhostBlue.wav"); 
					if (i < currentMap.ghostX.size()) 
					{
						ghost->setX(currentMap.ghostX[i]);
						ghost->setY(currentMap.ghostY[i]);
						ghost->setState(GhostState::CHASE);

					}
				}
				else
				{
					mPacman->decreaseLives();
					SoundManager::playSFX("Fail.wav");

					if (mPacman->getLives() <= 0)
					{
						//updateHighScore(mCurrentScore);
						mLevelRun = false;
						//mCurrentGameState = GameState::Menu;
					}
					else
					{
						mPacman->setX(currentMap.pacmanX);
						mPacman->setY(currentMap.pacmanY);
						mPacman->setCurDirection(Direction::NONE);
						mPacman->setNextDirection(Direction::NONE);

						for (int i = 0; i < mGhosts.size(); i++)
						{
							if (mGhosts[i] != nullptr)
							{
								mGhosts[i]->setX(currentMap.ghostX[i]);
								mGhosts[i]->setY(currentMap.ghostY[i]);
							}
						}
						system("cls");
						Sleep(1000);
					}
					break;
				}
			}
		
	}
}

void GameManager::cleanLevel()
{
	//Delete all pointers
	delete mPacman;
	mPacman = nullptr;

	for (Ghost* ghost : mGhosts) 
	{
		delete ghost;
	}
	mGhosts.clear();
}


