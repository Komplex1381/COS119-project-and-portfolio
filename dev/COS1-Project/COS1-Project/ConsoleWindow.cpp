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
