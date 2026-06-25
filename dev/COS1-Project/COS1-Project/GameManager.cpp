#include "gameManager.h"

GameManager::GameManager() : currentMap(0,0)
{
	ConsoleWindow::initialize(L"PAC-MAN P&P Edition", 80, 85);
	mWindow = new ConsoleWindow(80, 85);

	mCurrentGameState = GameState::Menu;
	mGameRun = true;
	mCurrentScore = 0;
	mHighScore = 0;
	mCurrentLevel = 1;
	mCurrentRound = 1;
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

void GameManager::killSequids(int targetX, int TargetY)
{
	for (int i = static_cast<int>(mGhosts.size()) - 1; i >= 0; i--) 
	{
		Ghost* ghost = mGhosts[i];
		if(ghost != nullptr && ghost->getX() == targetX && ghost->getY() == TargetY)
		{
			if(mChosenDifficulty == DifficultyChoice::Viltrumite)
			{
				delete ghost;
				mGhosts.erase(mGhosts.begin() + i);
				mCurrentScore += 100;
			}
			else if (ghost->getState() != GhostState::EATEN)
			{
				ghost->setState(GhostState::EATEN);
				mCurrentScore += 100;
			}
		}
	}
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
		bool topFiveScore = (highScores.size() < 5 || finalScore > highScores.back()->score);
		std::string playerInitials = "PAC";

		if (topFiveScore)
		{
			if (mWindow != nullptr) 
			{
				mWindow->clearScreen();
			}

			ConsoleWindow::setCursorPosition(5,5);
			std::cout << YELLOW << "NEW HIGH SCORE!" << GREEN << finalScore << RESET << "\n";
			ConsoleWindow::setCursorPosition(5,6);
			std::cout << WHITE << "ENTER YOUR INITIALS: " << YELLOW;

			//Helper::ClearInputBuffer();
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			std::string userInput = "";
			
			//Take in 3 int from user.
			while (userInput.length() < 3)
			{
				char letter = _getch();
				if (isalpha(letter))
				{
					letter = toupper(letter);
					userInput += letter;
					std::cout << letter;
				}
				else if (letter == '\b' && !userInput.empty())//backspace if they enter wrong
				{
					userInput.pop_back();
					std::cout << "\b\b";
				}
			}
			
			if (!userInput.empty()) 
			{
				playerInitials = userInput;
			}
			
		}
		

		Highscore* newEntry = new Highscore(finalScore, playerInitials);
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
			//saveScores();
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
	std::cout << "\n";
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
	bool redraw = true;

	while (stateRunning && mCurrentGameState == GameState::Difficulty)
	{
		if (redraw)
		{
			
			mWindow->clearScreen();
			Helper::TitleC("DIFFICULTY", WHITE, YELLOW);
			std::cout << BLUE << "Please select Difficulty: \n" << RESET;
			
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
			std::cout << RED << "Secret " << RESET << "Game Mode : " << YELLOW << "Viltrumite" << RESET << ". Punch = SPACEBAR, Infinity Ray = I\n";
			std::cout << "Press ESC to return to Menu...";
			redraw = false;
		}
		Sleep(30);
		
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			currentSelection = (currentSelection - 1 + 4) % 4;			
			redraw = true;
			Sleep(150);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			currentSelection = (currentSelection + 1) % 4;
			redraw = true;			
			Sleep(150);
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			mChosenDifficulty = static_cast<DifficultyChoice>(currentSelection);
			mCurrentGameState = GameState::Menu;
			stateRunning = false;			
			Sleep(150);
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			mCurrentGameState = GameState::Menu;
			stateRunning = false;			
			Sleep(150);
		}		
		
	}
	
}

void GameManager::initialLevel()
{
	int currentLives = (mPacman != nullptr) ? mPacman->getLives() : 3;
	
	mLevelRun = true;
	mFruitTimer = 0;

	//Reset wave counters for Viltrumite mode
	mCurrentWave = 1;
	mGhostsSpawnedInWave = 0;
	mWaveSpawnTimer = 0;
	mWaveSpeedModifier = 1.0;	

	////ConsoleWindow::initialize(L"PAC-MAN P&P Edition", 80, 80);
	if (mCurrentRound == 1) 
	{
		cleanLevel();
		mExitGhostHouse.clear();
		if (mCurrentLevel == 1)
		{
			mCurrentScore = 0;
		}

		//load map
		std::string mapFileName = "lvl" + std::to_string(mCurrentLevel) + ".txt";
		currentMap = MapLoader::loadMap(mapFileName);
		if (mapFileName == "lvl1.txt")
		{
			mapName = 1;
		}
		else if (mapFileName == "lvl2.txt")
		{
			mapName = 2;
		}
		else 
		{
			mapName = 3;
		}

		if (currentMap.getRows() == 0 || currentMap.getCols() == 0)
		{
			std::cout << "\nFailed to load Map: " << mapFileName << "\n";
			std::cout << "Loading previous Map.\n";
			Sleep(3000);

			int previousMapLevel = (mCurrentLevel > 1) ? (mCurrentLevel - 1) : 1;
			std::string previousMap = "lvl" + std::to_string(previousMapLevel) + ".txt";
			currentMap = MapLoader::loadMap(previousMap);
			mBaseGhostSpeed += 0.05;//Increase difficulty
		}
		
	}
	else 
	{
		currentMap.resetPellets();
	}
	if (mPacman != nullptr)
	{
		delete mPacman;
		mPacman = nullptr;
	}
	for (Ghost* ghost : mGhosts)
	{
		delete ghost;
	}
	mGhosts.clear();	

	ConsoleWindow::fitConsoleToMap(currentMap.getRows(), currentMap.getCols());
	

	if (mWindow != nullptr) 
	{
		mWindow->clearScreen();
	}
	
	//Scale difficulty	
	
	mBaseGhostSpeed = 0.6 + (mCurrentRound - 1) * 0.05;//Increase speed every round
	size_t ghostSpawnCount = currentMap.ghostX.size();

	//configure difficulty
	switch (mChosenDifficulty)
	{
	case DifficultyChoice::Easy:		
		mBaseGhostSpeed = 0.45;
		break;
	case DifficultyChoice::Normal:		
		mBaseGhostSpeed = 0.6;
		break;
	case DifficultyChoice::Hard:		
		mBaseGhostSpeed = 0.75;
		break;
	case DifficultyChoice::Viltrumite:
		currentLives = 1;
		mBaseGhostSpeed = 0.75;
		ghostSpawnCount = currentMap.ghostX.empty() ? 4: 20;
		break;
	default:
		break;
	}
	int spacing = (mChosenDifficulty == DifficultyChoice::Easy) ? 45 : 30;	

	//Pacman & ghost logic here
	mPacman = new Pacman(currentMap.pacmanX, currentMap.pacmanY, 0.8);
	mPacman->setLives(currentLives);

	WORD ghostColors[4] = 
	{ 
		FOREGROUND_RED | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
	};
	//Spawning loop
	for (size_t i = 0; i < ghostSpawnCount; i++)
	{
		size_t coordIndex = i % currentMap.ghostX.size();
		WORD color = ghostColors[i % 4];

		if (mChosenDifficulty == DifficultyChoice::Viltrumite) 
		{
			Sequids* newSequid = new Sequids(currentMap.ghostX[coordIndex], currentMap.ghostY[coordIndex], mBaseGhostSpeed, color);
			newSequid->setSpeed(mWaveSpeedModifier);
			mGhosts.push_back(newSequid);
		}
		else 
		{
			mGhosts.push_back(new Ghost(currentMap.ghostX[coordIndex], currentMap.ghostY[coordIndex], mBaseGhostSpeed, 'G', color));
		}
		mExitGhostHouse.push_back(static_cast<int>(i) * spacing);
	}
	
	currentMap.renderASCII();
	if (mPacman != nullptr)
	{
		ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());
		mPacman->draw();
	}

	//Only play intro at start of a map
	if (mCurrentRound == 1) 
	{
		SoundManager::playSFX("StartMusic.wav");

		Sleep(4200);
	}
	else 
	{
		Sleep(1000);
	}

}

void GameManager::gameplayLoop()
{

	while (mCurrentGameState == GameState::Start) 
	{
		initialLevel();

		if (mWindow != nullptr)
		{
			mWindow->clearScreen();
		}

		mPowerPellet = false;
		mPowerPelletTimer = 0;
		mGhostStateTimer = 0;
		mGhostHouseTimer = 0;

		while (mLevelRun)
		{

			mGhostStateTimer++;
			mGhostHouseTimer++;

			int curentCycle = mGhostStateTimer % 270;

			//Handle Scatter(7secs) & Chase(20secs) conditions 
			if (mGhostStateTimer == 200)
			{
				for (Ghost* ghost : mGhosts)
				{
					if (ghost != nullptr && ghost->getState() == GhostState::CHASE)
					{
						ghost->setState(GhostState::SCATTER);
					}
				}
			}
			else if (mGhostStateTimer == 270)
			{
				for (Ghost* ghost : mGhosts)
				{
					if (ghost != nullptr && ghost->getState() == GhostState::SCATTER)
					{
						ghost->setState(GhostState::CHASE);
					}
				}
				mGhostStateTimer = 0; //Reset timer
			}

			//Fruit timer
			mFruitTimer++;
			if (!mShowFruit && mFruitTimer >= mSpawnFruit)
			{
				currentMap.setRuntimeTile(currentMap.fruitY, currentMap.fruitX, 5);
				mShowFruit = true;
				mFruitTimer = 0;

				ConsoleWindow::setCursorPosition(currentMap.fruitX, currentMap.fruitY);
				std::cout << RED << "%" << RESET;
			}
			else if (mShowFruit && mFruitTimer >= mFruitVanish)
			{
				if (currentMap.getTile(currentMap.fruitY, currentMap.fruitX) == 5)
				{
					currentMap.setTile(currentMap.fruitY, currentMap.fruitX, 0);
					ConsoleWindow::setCursorPosition(currentMap.fruitX, currentMap.fruitY);
					std::cout << " ";
				}
				mShowFruit = false;
				mFruitTimer = 0;
			}
			//Power pellet timer
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
			if (mPacman != nullptr)
			{
				mPacman->handleInput();
			}

			updateGame();
			checkCollisions();

			//Fullscreen toggle
			if (GetAsyncKeyState(VK_F11) & 0x8000)
			{
				//flip variable
				mFullscreen = !mFullscreen;

				ConsoleWindow::setConsoleFullscreen(mFullscreen);

				if (!mFullscreen)
				{
					ConsoleWindow::fitConsoleToMap(currentMap.getRows(), currentMap.getCols());
				}
				Sleep(250);
			}


			//Return to Main menu
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				mLevelRun = false;
				mCurrentGameState = GameState::Menu;
			}
			renderGame();
			Sleep(100);
		}


		if (mWindow != nullptr)
		{
			mWindow->clearScreen();
		}

		if (mCurrentGameState != GameState::Start)
		{
			mCurrentGameState = GameState::Menu;
			updateHighScore(mCurrentScore);
			cleanLevel();
		}
		else {}


	}
}

void GameManager::updateGame()
{
	mIsAttacking = false;
	mActiveRayTiles = 0;
	//player & ghost update here
	if (mPacman != nullptr)
	{
		//return;
		ConsoleWindow::setCursorPosition(mPacman->getX(), mPacman->getY());
		std::cout << " ";
	}

	//Sequid wave Spawner
	if (mChosenDifficulty == DifficultyChoice::Viltrumite && mCurrentWave < 4) 
	{
		mWaveSpawnTimer++;
		if (mWaveSpawnTimer >= 300) 
		{
			mWaveSpawnTimer = 0;
			mCurrentWave++;
			mWaveSpeedModifier += 0.15;
			WORD sequidColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

			for (size_t i = 0; i < 20; i++)
			{
				size_t coordIndex = i % currentMap.ghostX.size();
				double dynamicSpeed = mBaseGhostSpeed * mWaveSpeedModifier;

				Sequids* sequidWave = new Sequids(currentMap.ghostX[coordIndex], currentMap.ghostY[coordIndex], dynamicSpeed, sequidColor);
				sequidWave->SetWaveSpeed(mWaveSpeedModifier);

				mGhosts.push_back(sequidWave);
				mExitGhostHouse.push_back(i * 30);//Spacing
			}
			//SoundManager::playSFX("");
		}

		
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
	/////////////////////////////////
	//Attack for Pacman Invincible //
	/////////////////////////////////
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
		killSequids(attackX, attackY);
		
	}

	//bool isViltrumiteMode = (mChosenDifficulty == DifficultyChoice::Viltrumite);
	////////////////
	//Infinity Ray//
	////////////////
	if (mPacman != nullptr && mChosenDifficulty == DifficultyChoice::Viltrumite && (GetAsyncKeyState('I') & 0x8000))
	{
		//mIsBeamActive = true;
		if (!mIsBeamActive) 
		{
			Direction currentFacing = mPacman->getCurrentDirection();

			if (currentFacing == Direction::NONE)
			{
				currentFacing = Direction::RIGHT;
			}	

			
			mBeamX = mPacman->getX();
			mBeamY = mPacman->getY();
			mBeamDirection = currentFacing;


			switch (mBeamDirection)
			{
			case Direction::UP:
				mBeamY--;
				break;
			case Direction::DOWN:
				mBeamY++;
				break;
			case Direction::LEFT:
				mBeamX--;
				break;
			case Direction::RIGHT:
				mBeamX++;
				break;
			default:
				break;
			}
			//Check map boundry
			if (mBeamX >= 0 && mBeamX < currentMap.getCols() && mBeamY >= 0 && mBeamY < currentMap.getRows())
			{
				if (currentMap.getTile(mBeamY, mBeamX) != 3)
				{
					mIsBeamActive = true;
					SoundManager::playSFX("laser.wav");
				}
			}
			
						
		}
		//Direction currentFacing = mPacman->getCurrentDirection();

		//if (currentFacing == Direction::NONE) 
		//{
		//	currentFacing = Direction::RIGHT;
		//}
		//mIsBeamActive = true;

		//for (int step = 1; step <= 3; step++)
		//{
		//	int rayX = mPacman->getX();
		//	int rayY = mPacman->getY();


		//	switch (currentFacing)
		//	{
		//	case Direction::UP:
		//		rayY -= step;
		//		break;
		//	case Direction::DOWN:
		//		rayY += step;
		//		break;
		//	case Direction::LEFT:
		//		rayX -= step;
		//		break;
		//	case Direction::RIGHT:
		//		rayX += step;
		//		break;
		//	default:
		//		break;
		//	}
		//	//Check map boundry
		//	if (rayX < 0 || rayX >= currentMap.getCols() || rayY < 0 || rayY >= currentMap.getRows())
		//	{
		//		break;
		//	}
		//	//Wall block
		//	if (currentMap.getTile(rayY, rayX) == 3) 
		//	{
		//		break;
		//	}
		//	//protection against array limits
		//	if (mActiveRayTiles < 4)
		//	{
		//		mInfinityRayX[mActiveRayTiles] = rayX;
		//		mInfinityRayY[mActiveRayTiles] = rayY;
		//		mActiveRayTiles++;
		//	}
		//	killSequids(rayX, rayY);
		//	
		//}
	}
	if (mIsBeamActive) 
	{
		switch (mBeamDirection)
		{
		case Direction::UP:
			mBeamY--;
			break;
		case Direction::DOWN:
			mBeamY++;
			break;
		case Direction::LEFT:
			mBeamX--;
			break;
		case Direction::RIGHT:
			mBeamX++;
			break;
		default:
			mIsBeamActive = false;
			break;
		}

		if (mBeamX < 0 || mBeamX >= currentMap.getCols() || mBeamY < 0 && mBeamY >= currentMap.getRows()) 
		{
			mIsBeamActive = false;
		}
		else if (currentMap.getTile(mBeamY, mBeamX) == 3)
		{
			mIsBeamActive = false;
		}
		else 
		{
			bool hit = false;
			for (Ghost* ghost : mGhosts) 
			{
				if (ghost != nullptr && ghost->getX() == mBeamX && ghost->getY() == mBeamY) 
				{
					if (ghost->getState() != GhostState::EATEN) 
					{
						killSequids(mBeamX, mBeamY);
						hit = true;
					}
				}
			}
			if (hit) 
			{
				mIsBeamActive = false;
			}
		}
	}
	
	int doorX = 13; //currentMap.getCols() / 2;
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
			bool insideHouse = (ghost->getY() > 10);//was 11

			int ghostRelease = (i < mExitGhostHouse.size()) ? mExitGhostHouse[i] : 0;

			if (insideHouse && mGhostHouseTimer < ghostRelease && ghost->getState() != GhostState::FRIGHTENED && ghost->getState() != GhostState::EATEN)
			{
				ghost->updateAI(currentMap, 13, 12, mPacman->getCurrentDirection(), blinkyX, blinkyY);
			}
			else if (insideHouse)
			{
				//ghost->updateAI(currentMap, doorX, doorY, mPacman->getCurrentDirection(), blinkyX, blinkyY);
				if (ghost->getX() != 13) 
				{
					ghost->updateAI(currentMap, 13, ghost->getY(), mPacman->getCurrentDirection(), blinkyX, blinkyY);
				}
				else 
				{
					ghost->updateAI(currentMap, 13, 9, mPacman->getCurrentDirection(), blinkyX, blinkyY);
				}
			}
			else
			{
				ghost->updateAI(currentMap, mPacman->getX(), mPacman->getY(), mPacman->getCurrentDirection(), blinkyX, blinkyY);
			}
			ghost->decreaseMovement();
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
	WORD red = FOREGROUND_RED | FOREGROUND_INTENSITY;
	WORD brown = FOREGROUND_RED | FOREGROUND_GREEN;
	WORD blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	WORD flashColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	WORD rayColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

	

	//Draw HUD Header - moved below score
	
	for (int y = 0; y < mapRows; y++) 
	{
		for (int x = 0; x < mapCols; x++)
		{
			char tile = currentMap.getTile(y, x);
			int renderY = y; 

			if (tile == 1)
			{
				
				if (mapName == 1)
				{
					mWindow->Draw(x, renderY, '.', white);
				}
				else if (mapName == 2)
				{
					mWindow->Draw(x, renderY, '.', brown);
				}
				else
				{
					mWindow->Draw(x, renderY, '.', red);
				}
			}
			else if (tile == 2)
			{
				mWindow->Draw(x, renderY, 'O', yellow);
			}
			else if (tile == 3)
			{
				if (mapName == 1) 
				{ 
					mWindow->Draw(x, renderY, '#', blue);
				}
				else if (mapName == 2)
				{
					mWindow->Draw(x, renderY, '#', flashColor);
				}
				else 
				{
					mWindow->Draw(x, renderY, '#', brown);
				}
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
	if (mIsBeamActive) 
	{
		char beamCharacter = (mBeamDirection == Direction::LEFT || mBeamDirection == Direction::RIGHT) ? '=' : '|';
		mWindow->Draw(mBeamX, mBeamY, beamCharacter, rayColor);
		
		
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
		
		//Invinible Visual Attacks				
		if (mIsAttacking)
		{
			if (mAttackVisualX >= 0 && mAttackVisualY >= 0)
			{
				//Punch 1 tile ahead			
				mWindow->Draw(mAttackVisualX, mAttackVisualY, 'X', flashColor);
			}

			//if (mChosenDifficulty == DifficultyChoice::Viltrumite)
			//{
			//	//InfinityRay 3 tiles ahead				
			//	for (int i = 0; i < mActiveRayTiles; i++)
			//	{
			//		mWindow->Draw(mInfinityRayX[i], mInfinityRayY[i], '>', rayColor);
			//	}
			//}
		}
	}
	mWindow->Display();

	//Draw HUD Footer
	int BottumHud = mapRows;
	ConsoleWindow::setCursorPosition(0, BottumHud);	
	std::cout << WHITE << "LIVES: ";
	
	int totalLives = (mPacman != nullptr) ? mPacman->getLives() : 0;
	for (int i = 0; i < totalLives; i++) 
	{
		std::cout << YELLOW << "C " << RESET;
	}

	std::cout << WHITE << "           ";
	ConsoleWindow::setCursorPosition(mapCols - 15, BottumHud);
	std::cout << YELLOW << "  FRUIT: " << RED << "%" << RESET << "    \n";
	
	//Display Score & Highscore
	int hudScoreY = BottumHud + 1;
	std::string score = "Score: " + std::to_string(mCurrentScore);
	for (size_t i = 0; i < score.length(); i++)
	{
		mWindow->Draw(static_cast<int>(i), hudScoreY, score[i], yellow);
	}
	
	std::string hiScore = "HIGH SCORE: " + std::to_string(mHighScore);
	int hiScoreX = mapCols - static_cast<int>(hiScore.length());
	if (hiScoreX > 0)
	{
		for (size_t i = 0; i < hiScore.length(); i++)
		{
			mWindow->Draw(hiScoreX + static_cast<int>(i), hudScoreY, hiScore[i], FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
	}
	//Display Score
	ConsoleWindow::setCursorPosition(0, hudScoreY);
	std::cout << WHITE << "Score: " << YELLOW << mCurrentScore;	
	
	//Display highscore
	ConsoleWindow::setCursorPosition(hiScoreX, hudScoreY);
	std::cout << WHITE << "HIGH SCORE: " << RED << mHighScore << "\n";
	RESET;
	
	ConsoleWindow::setCursorPosition(0, 0);
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
		//SoundManager::playSFX("Eating.wav");

	}
	else if (currentTile == 2) //POWER Pellet
	{
		currentMap.setTile(pacY, pacX, 0);
		mCurrentScore += 50;

		mPowerPellet = true;
		mPowerPelletTimer = 50;
		SoundManager::playSFX("GhostBlue");

		for (Ghost* ghost : mGhosts)
		{
			if (ghost != nullptr)
			{
				ghost->setState(GhostState::FRIGHTENED);				
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

		if (!collision && !overlap)
		{
			continue;
		}
		if (ghost->getState() == GhostState::FRIGHTENED) 
		{
			mCurrentScore += 200;
			//Sound effect
			SoundManager::playSFX("GhostBlue.wav");
			ghost->setState(GhostState::EATEN);
					
		}
		else if(ghost->getState() != GhostState::EATEN)
		{
			playerDeath();			
			return;
		}
			
		
	}
	//Clear the map
	if (currentMap.clearedPellets()) 
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

		Sleep(2000);
		
		mCurrentScore += 1000;

		//Advance to next round
		if (mCurrentRound == 1 || mCurrentRound == 2)
		{
			mCurrentRound++;			
			mCurrentGameState = GameState::Start;
		}
		else 
		{
			mCurrentRound = 1;
			mCurrentLevel++;
			mCurrentGameState = GameState::Start;
		}
		return;
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
void GameManager::playerDeath()
{
	mPacman->decreaseLives();
	SoundManager::playSFX("Fail.wav");

	if (mPacman->getLives() <= 0)
	{
		mLevelRun = false;
		mWindow->ClearBuffer();
		renderGame();

		std::string gameOverText = "  GAME OVER  ";
		int bannerX = (currentMap.getCols() / 2) - (static_cast<int>(gameOverText.length()) / 2);
		int bannerY = currentMap.getRows() / 2;

		WORD blackBackground = 0;
		for (int offset = -2; offset < static_cast<int>(gameOverText.length()) + 2; offset++)
		{
			mWindow->Draw(bannerX + offset, bannerY - 1, ' ', blackBackground);
			mWindow->Draw(bannerX + offset, bannerY, ' ', blackBackground);
			mWindow->Draw(bannerX + offset, bannerY + 1, ' ', blackBackground);
		}
		WORD deadColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		for (size_t i = 0; i < gameOverText.length(); i++)
		{
			mWindow->Draw(bannerX + static_cast<int>(i), bannerY, gameOverText[i], deadColor);

		}
		mWindow->Display();
		Sleep(3000);

		mCurrentRound = 1;	
		mCurrentLevel = 1;
		mCurrentScore = 0;
		mCurrentGameState = GameState::Menu;

		delete mPacman;
		mPacman = nullptr;

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
		mGhostHouseTimer = 0;
		mPowerPellet = false;
		mPowerPelletTimer = 0;

		mWindow->ClearBuffer();
		mWindow->Display();
		Sleep(1000);
	}
}


