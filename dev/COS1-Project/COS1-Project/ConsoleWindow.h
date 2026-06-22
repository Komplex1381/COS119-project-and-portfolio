#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

class ConsoleWindow
{
private:
	int mScreenWidth;
	int mScreenHeight;
	HANDLE mConsole;
	CHAR_INFO* mScreenBuffer;
	SMALL_RECT mWindowRect;
public:
	static void initialize(const std::wstring& title, int width, int height);
	static void setConsoleFullscreen(bool fullscreen);
	static void fitConsoleToMap(int mapRows, int mapCols);
	static void hideConsoleCursor();
	static void setCursorPosition(int x, int y);
	int getWidth() const;
	ConsoleWindow(int width, int height);
	~ConsoleWindow();
	void ClearBuffer();
	void Draw(int x, int y, char c, WORD color);	
	void Display();
	void clearScreen();
};

