#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

class ConsoleWindow
{
public:
	static void initialize(const std::wstring& title, int width, int height);
};

