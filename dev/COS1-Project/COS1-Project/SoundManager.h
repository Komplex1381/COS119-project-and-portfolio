#pragma once
#include <string>
#include <windows.h>
#include <mmsystem.h>

class SoundManager
{
public:
	static void playSFX(const std::string& filename);
	static void playLoop(const std::string& filename);
	static void stop();
};

