#pragma once

#include "Board.h"

class AI
{
public:

	char mark;

	AI();
	~AI();
	int minimax(Board *b, char m);
	int pickPosition(Board *b);
};

