#include "ConsoleWindow.h"


void ConsoleWindow::initialize(const std::wstring& title, int width, int height)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hwnd = GetConsoleWindow();

	if (hConsole == INVALID_HANDLE_VALUE || hwnd == NULL)
	{
		return;
	}

	SetConsoleTitleW(title.c_str());

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 32; // Width of each character cell
	cfi.dwFontSize.Y = 32; // Height of Character cell
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); //Font here
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	//Screen buffer size
	COORD bufferSize = { static_cast<short>(width), static_cast<short>(height) };
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	//Adjust window size
	SMALL_RECT windowSize = { 0,0, static_cast < short>(width - 1), static_cast < short>(height - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX; //Remove maximize button
	style &= ~WS_THICKFRAME; //Remove drag-resize border
	SetWindowLong(hwnd, GWL_STYLE, style);

	SetWindowPos(hwnd, NULL,0, 0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

//Windows code for Cursor
void ConsoleWindow::setCursorPosition(int x, int y)
{
	COORD coord = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleWindow::hideConsoleCursor()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(out, &cursorInfo);
}

//Attempt to make game window map size(updated because afer checking again I was doing the function wrong, I need to also buffer the window as well as buffer)
void ConsoleWindow::fitConsoleToMap(int mapRows, int mapCols)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
	{
		return;
	}

	//Space for Hud
	short paddingWidth = 5;
	short paddingHeight = 8;

	short finalWidth = static_cast<short>(mapCols + paddingWidth);
	short finalHeight = static_cast<short>(mapRows + paddingHeight);

	//Keeps window from exceeding desktop
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		if (finalWidth > csbi.dwMaximumWindowSize.X) finalWidth = csbi.dwMaximumWindowSize.X;
		if (finalHeight > csbi.dwMaximumWindowSize.Y) finalHeight = csbi.dwMaximumWindowSize.Y;
	}
	//Shrinks window size
	SMALL_RECT minimalWin = { 0,0, 1,1 };
	SetConsoleWindowInfo(hConsole, TRUE, &minimalWin);
	//Resize the screen buffer
	COORD bufferSize = { finalWidth, finalHeight };
	SetConsoleScreenBufferSize(hConsole, bufferSize);
	//Expand the view window
	SMALL_RECT windowSize = { 0,0, static_cast<short>(finalWidth - 1), static_cast<short>(finalHeight - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void ConsoleWindow::setConsoleFullscreen(bool fullscreen)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD flags = fullscreen ? CONSOLE_FULLSCREEN_MODE : CONSOLE_WINDOWED_MODE;
	COORD newSize;

	SetConsoleDisplayMode(hConsole, flags, &newSize);
}


ConsoleWindow::ConsoleWindow(int width, int height) : mScreenWidth(width), mScreenHeight(height)
{
	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	mScreenBuffer = new CHAR_INFO[mScreenWidth * mScreenHeight];

	mWindowRect = { 0,0,(short)(mScreenWidth - 1), (short)(mScreenHeight - 1) };
	COORD bufferSize = { (short)mScreenWidth, (short)mScreenHeight };

	SetConsoleScreenBufferSize(mConsole, bufferSize);
	SetConsoleWindowInfo(mConsole, TRUE, &mWindowRect);

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(mConsole, &cursorInfo);

	ClearBuffer();
}

ConsoleWindow::~ConsoleWindow()
{
	delete[] mScreenBuffer;
}

void ConsoleWindow::ClearBuffer()
{
	for (int i = 0; i < ConsoleWindow::mScreenWidth * ConsoleWindow::mScreenHeight; i++)
	{
		mScreenBuffer[i].Char.AsciiChar = ' ';
		mScreenBuffer[i].Attributes = 0;
	}
}

void ConsoleWindow::Draw(int x, int y, char c, WORD color = FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED)
{
	if(x >= 0 && x < mScreenWidth && y >= 0 && y < mScreenHeight) 
	{
		mScreenBuffer[y * mScreenWidth + x].Char.AsciiChar = c;
		mScreenBuffer[y * mScreenWidth + x].Attributes = color;
	}
}

void ConsoleWindow::Display()
{
	COORD bufferSize = { (short)mScreenWidth, (short)mScreenHeight };
	COORD bufferCoord = { 0,0 };
	WriteConsoleOutputA(mConsole, mScreenBuffer,bufferSize,bufferCoord, &mWindowRect);
}


