#pragma once

#include <iostream>
#include <sstream>
#include <SDL.h>

class Board
{
public:

	char board[9];
	int emptySpace;

	Board();
	~Board();
	void draw();
	void drawGL(int w, int h, void (*dispTxt)(std::string txt, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB));
	void put(int position, char mark);
	bool checkAvailability(int position);
	bool checkEndGameConditions();
	bool checkPlayerWin(char mark);
	int countBlank();
};

