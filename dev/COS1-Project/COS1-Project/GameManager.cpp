#include "gameManager.h"
#include <Windows.h>
#include <iostream>


GameManager::GameManager() : currentMap(0,0)
{
	ConsoleWindow::initialize(L"PAC-MAN P&P Edition", 80, 85);
	mWindow = new ConsoleWindow(80, 85);

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
	int currentSelection = 0;
	bool stateRunning = true;
	while (stateRunning && mCurrentGameState == GameState::Difficulty)
	{
		system("cls");
		Helper::TitleC("DIFFICULTY", WHITE, YELLOW);
		std::cout << BLUE << "Please select Difficulty: \n" << RESET;
		//This will change later once I get game running and add feature.
		std::string choices[] = { "Easy", "Normal", "Hard", "Viltrumite" };
		for (int i = 0; i < 4; i++)
		{
			if (currentSelection == i) 
			{
				std::cout << YELLOW << " > " << choices[i] << " <\n" << RESET;
			}
			else 
			{
				std::cout << WHITE << "  " << choices[i] << "\n" << RESET;
			}
		}
		std::cout << "\nPress UP/DOWN to scroll, ENTER to select.\n";
		std::cout << "Press ESC to return to Menu...";

		bool keyPressed = false;
		while (!keyPressed)
		{
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				currentSelection = (currentSelection + 4) % 4;
				keyPressed = true;
				Sleep(150);
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				currentSelection = (currentSelection + 1) % 4;
				keyPressed = true;
				Sleep(150);
			}
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			{
				mChosenDifficulty = static_cast<DifficultyChoice>(currentSelection);
				mCurrentGameState = GameState::Menu;
				stateRunning = false;
				keyPressed = true;
				Sleep(150);
			}
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				mCurrentGameState = GameState::Menu;
				stateRunning = false;
				keyPressed = true;
				Sleep(150);
			}
		}
		Sleep(10);
	}
	
}

void GameManager::initialLevel()
{
	cleanLevel();
	mExitGhostHouse.clear();
	ConsoleWindow::initialize(L"PAC-MAN P&P Edition", 80, 80);
	mCurrentScore = 0;
	mLevelRun = true;
	mFruitTimer = 0;
	//load map
	std::string mapFileName = "lvl" + std::to_string(mCurrentLevel) + ".txt";
	currentMap = MapLoader::loadMap(mapFileName);
	if (currentMap.getRows() == 0 || currentMap.getCols() == 0)
	{
		currentMap = MapLoader::loadMap("lvl2.txt");
		mBaseGhostSpeed += 0.05;
	}

	ConsoleWindow::fitConsoleToMap(currentMap.getRows(), currentMap.getCols());
	system("cls");
	
	int startingLives = 3;
	mBaseGhostSpeed = 0.6;
	size_t ghostSpawnCount = currentMap.ghostX.size();

	switch (mChosenDifficulty)
	{
	case DifficultyChoice::Easy:
		startingLives = 4;
		mBaseGhostSpeed = 0.45;
		break;
	case DifficultyChoice::Normal:
		startingLives = 3;
		mBaseGhostSpeed = 0.6;
		break;
	case DifficultyChoice::Hard:
		startingLives = 2;
		mBaseGhostSpeed = 0.75;
		break;
	case DifficultyChoice::Viltrumite:
		startingLives = 1;
		mBaseGhostSpeed = 0.75;
		ghostSpawnCount = 80;
		break;
	default:
		break;
	}
	int spacing = 30;
	if (mChosenDifficulty == DifficultyChoice::Easy) { spacing = 45; }
	//Pacman & ghost logic here
	mPacman = new Pacman(currentMap.pacmanX, currentMap.pacmanY, 0.8);
	mPacman->setLives(startingLives);

	WORD ghostColors[4] = 
	{ 
		FOREGROUND_RED | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
	};
	for (size_t i = 0; i < ghostSpawnCount; i++)
	{
		size_t coordIndex = i % currentMap.ghostX.size();
		WORD color = ghostColors[i % 4];

		mGhosts.push_back(new Ghost(currentMap.ghostX[coordIndex], currentMap.ghostY[coordIndex], mBaseGhostSpeed, 'G', color));
		mExitGhostHouse.push_back(static_cast<int>(i) * spacing);
	}
	
	currentMap.renderASCII();
	if (mPacman != nullptr)
	{
		ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());
		mPacman->draw();
	}

	////Draw Ghost
	//for(size_t i  = 0; i < currentMap.ghostX.size() && i < 4; i++) 
	//{
	//	mGhosts.push_back(new Ghost(currentMap.ghostX[i], currentMap.ghostY[i], 0.6, 'G', ghostColors[i]));
	//}
	//for (const Ghost* ghost : mGhosts)
	//{
	//	if (ghost != nullptr)
	//	{
	//		ConsoleWindow::setCursorPosition(ghost->getX(), ghost->getY());
	//		ghost->draw();
	//	}
	//}

	SoundManager::playSFX("StartMusic.wav");

	Sleep(4200);

}

void GameManager::gameplayLoop()
{
	
	system("cls");	

	mPowerPellet = false;
	mPowerPelletTimer = 0;
	mGhostStateTimer = 0;
	mGhostHouseTimer = 0;

	while (mLevelRun) 
	{
		
		mGhostStateTimer++;
		mGhostHouseTimer++;

		
		for (Ghost* ghost : mGhosts)
		{
			if (mGhostStateTimer >= 200 && ghost->getState() == GhostState::CHASE)
			{
				ghost->setState(GhostState::SCATTER);
				mGhostStateTimer = 0;
			}
			else if (mGhostStateTimer >= 70 && ghost->getState() == GhostState::SCATTER) 
			{
				ghost->setState(GhostState::CHASE);
				mGhostStateTimer = 0;
			}
			
		}
			
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
	mIsAttacking = false;
	mActiveRayTiles = 0;
	//player & ghost update here
	if (mPacman != nullptr)
	{
		
		ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());
		std::cout << " ";
	}

	int blinkyX = 0;
	int blinkyY = 0;
	WORD blinkyColorCode = FOREGROUND_RED | FOREGROUND_INTENSITY;

	for (const Ghost* ghost : mGhosts)
	{
		if (ghost != nullptr)
		{

			ConsoleWindow::setCursorPosition(ghost->getX(), ghost->getY());
			char tileToShow = currentMap.getTile(ghost->getY(), ghost->getX());
			switch (tileToShow)
			{
			case 1://Pellet
			{
				std::cout << WHITE << ".";
				break;
			}
			case 2://Power pellet
			{
				std::cout << YELLOW << "O";
				break;
			}
			case 4://Door
			{
				std::cout << WHITE << "-";
				break;
			}
			case 5://Fruit
			{
				std::cout << RED << "%";
				break;
			}
			default://Empty space
			{
				std::cout << " ";
				break;
			}

			}


			if (ghost->getColor() == blinkyColorCode)
			{
				blinkyX = ghost->getX();
				blinkyY = ghost->getY();
				//break;
			}
		}
	}

	if (mPacman != nullptr)
	{
		mPacman->update(currentMap);
	}
	////Attack for Pacman Invincible
	if (mPacman != nullptr && mChosenDifficulty == DifficultyChoice::Viltrumite && (GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		int attackX = mPacman->getX();
		int attackY = mPacman->getY();
		switch (mPacman->getCurrentDirection()) 
		{
		case Direction::UP:
			attackY--;
			break;
		case Direction::DOWN:
			attackY++;
			break;
		case Direction::LEFT:
			attackX--;
			break;
		case Direction::RIGHT:
			attackX++;
			break;
		default:
			break;
		}
		mIsAttacking = true;
		mAttackVisualX = attackX;
		mAttackVisualY = attackY;
		for (Ghost* ghost : mGhosts)
		{
			if (ghost != nullptr && ghost->getX() == attackX && ghost->getY() == attackY)
			{
				if (ghost->getState() != GhostState::EATEN) 
				{
					ghost->setState(GhostState::EATEN);
					mCurrentScore += 100;
					//sound effect
				}
			}
		}
	}
	
	//Infinity Ray
	//if (mPacman != nullptr && mChosenDifficulty == DifficultyChoice::Viltrumite && (GetAsyncKeyState(VK_SPACE) & 0x8000))
	//{
	//	mIsAttacking = true;
	//	Direction currentFacing = mPacman->getCurrentDirection();

	//	if (currentFacing == Direction::NONE) 
	//	{
	//		currentFacing = Direction::RIGHT;
	//	}
	//	for (int step = 1; step <= 3; step++)
	//	{
	//		int rayX = mPacman->getX();
	//		int rayY = mPacman->getY();


	//		switch (currentFacing)
	//		{
	//		case Direction::UP:
	//			rayY -= step;
	//			break;
	//		case Direction::DOWN:
	//			rayY += step;
	//			break;
	//		case Direction::LEFT:
	//			rayX -= step;
	//			break;
	//		case Direction::RIGHT:
	//			rayX += step;
	//			break;
	//		default:
	//			break;
	//		}
	//		//Check map boundry
	//		if (rayX < 0 || rayX >= currentMap.getCols() || rayY < 0 || rayY >= currentMap.getRows())
	//		{
	//			break;
	//		}
	//		//Wall block
	//		if (currentMap.getTile(rayY, rayX) == 3) 
	//		{
	//			break;
	//		}
	//		
	//		for (Ghost* ghost : mGhosts)
	//		{
	//			if (ghost != nullptr && ghost->getX() == rayX && ghost->getY() == rayY)
	//			{
	//				if (ghost->getState() != GhostState::EATEN)
	//				{
	//					ghost->setState(GhostState::EATEN);
	//					mCurrentScore += 100;
	//					//sound effect
	//				}
	//			}
	//		}
	//	}
	//}

	//int exitGhostHouse[] = { 0,10,15,20 };
	int doorX = 13;//  currentMap.getCols() / 2;
	int doorY = 10; //(currentMap.getRows() / 2) - 1;

	for (size_t i = 0; i < mGhosts.size(); i++)
	{
		Ghost* ghost = mGhosts[i];
		if (ghost == nullptr || mPacman == nullptr)
		{
			continue;
		}

		if (ghost->getState() == GhostState::FRIGHTENED)
		{
			ghost->setSpeed(0.4);
		}
		else if (ghost->getState() == GhostState::EATEN)
		{
			ghost->setSpeed(1.5);
		}
		else
		{
			ghost->setSpeed(mBaseGhostSpeed);
		}

		ghost->increaseMovement();

		if (ghost->canMove())
		{
			bool insideHouse = (ghost->getY() > 11);

			if (insideHouse && mGhostHouseTimer < mExitGhostHouse[i] && ghost->getState() != GhostState::FRIGHTENED && ghost->getState() != GhostState::EATEN)
			{
				ghost->updateAI(currentMap, 13, 12, mPacman->getCurrentDirection(), blinkyX, blinkyY);
			}
			else if (insideHouse)
			{
				ghost->updateAI(currentMap, doorX, doorY, mPacman->getCurrentDirection(), blinkyX, blinkyY);
			}
			else
			{
				ghost->updateAI(currentMap, mPacman->getX(), mPacman->getY(), mPacman->getCurrentDirection(), blinkyX, blinkyY);
			}
			ghost->decreaseMovement();
		}
	}
	/*for (Ghost* ghost : mGhosts)
	{
		if (ghost != nullptr && mPacman != nullptr)
		{
			ghost->updateAI(currentMap, mPacman->getX(), mPacman->getY(), mPacman->getCurrentDirection(), blinkyX, blinkyY);
		}
	}*/
	
}

void GameManager::renderGame()
{
	mWindow->ClearBuffer();

	int mapRows = currentMap.getRows();
	int mapCols = currentMap.getCols();
	WORD white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	WORD yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

	

	//Draw HUD Header
	//std::string score = "Score: " + std::to_string(mCurrentScore);
	//for (size_t i = 0; i < score.length(); i++) 
	//{
	//	mWindow->Draw(static_cast<int>(i), 0, score[i], yellow);
	//}
	////std::cout << WHITE << "Score: " << YELLOW << mCurrentScore;
	////setCursorPosition(mapCols / 2, 0);
	//std::string hiScore = "HIGH SCORE: " + std::to_string(mHighScore);
	//int hiScoreX = mapCols - static_cast<int>(hiScore.length());
	//if (hiScoreX > 0) 
	//{
	//	for (size_t i = 0; i < hiScore.length(); i++)
	//	{
	//		mWindow->Draw(hiScoreX + static_cast<int>(i),0, hiScore[i], FOREGROUND_RED | FOREGROUND_INTENSITY);
	//	}
	//}
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
				mWindow->Draw(x, renderY, '.', white);
			}
			else if (tile == 2)
			{
				mWindow->Draw(x, renderY, 'O', yellow);
			}
			else if (tile == 3)
			{
				mWindow->Draw(x, renderY, '#', FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else if (tile == 4)
			{
				mWindow->Draw(x, renderY, '-', white);
			}
			else if (tile == 5)
			{
				mWindow->Draw(x, renderY, '%', FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			
		}
	}

	//Draw player on map
	if (mPacman != nullptr) 
	{
		mWindow->Draw(mPacman->getX(), mPacman->getY(), 'C', yellow);
	}

	//Draw Ghost
	for (const Ghost* ghost : mGhosts) 
	{
		if (ghost != nullptr) 
		{
			WORD ghostColor;
			char ghostChar = 'G';

			if (ghost->getState() == GhostState::EATEN) 
			{
				ghostColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				ghostChar = 'u';
			}
			else if (mPowerPellet)
			{
				ghostColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			}
			else 
			{
				ghostColor = ghost->getColor();
			}
			mWindow->Draw(ghost->getX(), ghost->getY(), ghostChar, ghostColor);
		}
		//Invinible Attacks
		//Punch 1 tile ahead
		if (mIsAttacking && mAttackVisualX >= 0 && mAttackVisualY >= 0) 
		{
			WORD flashColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			mWindow->Draw(mAttackVisualX, mAttackVisualY, 'X', flashColor);
		}
		//InfinityRay 3 tiles ahead
		/*if (mChosenDifficulty == DifficultyChoice::Viltrumite && mIsAttacking)
		{
			WORD rayColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			for (int i = 0; i < mActiveRayTiles; i++) 
			{ mWindow->Draw(mInfinityRayX[i], mInfinityRayY[i], '>', rayColor); }
		}*/
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
		//pellet.eaten();
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
				//ghost->setTile('@');
			}
		}
	}//Fruit
	else if (currentTile == 5)
	{
		Fruit cherry("Cherry", 100);		
		cherry.eaten();

		mCurrentScore += cherry.getPoints();
		currentMap.setTile(pacY, pacX, 0);

		mShowFruit = false;
		mFruitTimer = 0;
	}

	//Ghost Collision
	for (size_t i = 0; i < mGhosts.size(); i++)
	{
		Ghost* ghost = mGhosts[i];
		if (ghost == nullptr)
		{
			continue;
		}
			int ghostX = ghost->getX();
			int ghostY = ghost->getY();
			bool overlap = pacX == ghostX && pacY == ghostY;
			bool collision = pacX == ghost->getPreviousX() && pacY == ghost->getPreviousY() && mPacman->getPreviousX() == ghostX && mPacman->getPreviousY() == ghostY;

			if (collision || overlap)
			{
				if (ghost->getState() == GhostState::FRIGHTENED) 
				{
					mCurrentScore += 200;
					//Sound effect
					SoundManager::playSFX("GhostBlue.wav");
					ghost->setState(GhostState::EATEN);
					
				}
				else if(ghost->getState() != GhostState::EATEN)
				{
					mPacman->decreaseLives();
					SoundManager::playSFX("Fail.wav");

					if (mPacman->getLives() <= 0)
					{
						mLevelRun = false;
					}
					else//Reset positions
					{
						mPacman->setX(currentMap.pacmanX);
						mPacman->setY(currentMap.pacmanY);
						mPacman->setCurDirection(Direction::NONE);
						mPacman->setNextDirection(Direction::NONE);

						for (size_t j = 0; j < mGhosts.size(); j++)
						{
							if (mGhosts[j] != nullptr)
							{
								size_t coordIndex = j % currentMap.ghostX.size();

								mGhosts[j]->setX(currentMap.ghostX[coordIndex]);
								mGhosts[j]->setY(currentMap.ghostY[coordIndex]);
								mGhosts[j]->setState(GhostState::CHASE);
							}
						}
						mGhostStateTimer = 0;
						mPowerPellet = false;
						mPowerPelletTimer = 0;

						mWindow->ClearBuffer();
						mWindow->Display();
						Sleep(1000);
					}
					break;
				}
			}
		
	}
	if (!currentMap.clearedPellets()) 
	{
		mLevelRun = false;
		//SoundManager::stop();
		//SoundManager::playSFX();

		int middleScreenX = currentMap.getCols() / 2 - 5;
		int middleScreenY = currentMap.getRows() / 2;
		std::string victoryText = "Victory!";

		WORD winColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		for (size_t i = 0; i < victoryText.length(); i++) 
		{
			mWindow->Draw(middleScreenX + static_cast<int>(i), middleScreenY, victoryText[i], winColor);
		}
		mWindow->Display();

		Sleep(3000);
		mCurrentLevel++;
		mCurrentScore += 1000;
		mCurrentGameState = GameState::Start;
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


