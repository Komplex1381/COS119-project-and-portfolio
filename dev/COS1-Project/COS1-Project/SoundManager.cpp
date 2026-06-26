#include "SoundManager.h"


#pragma comment(lib, "winmm.lib")

void SoundManager::playSFX(const std::string& filename)
{
	PlaySoundA(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}

void SoundManager::playLoop(const std::string& filename)
{
	PlaySoundA(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
}

void SoundManager::stop()
{
	PlaySoundA(NULL, NULL, 0);
}
