#pragma once
#include <string>

class SoundManager
{
public:
	static void playSFX(const std::string& filename);
	static void playLoop(const std::string& filename);
	static void stop();
};

