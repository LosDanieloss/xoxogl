#include "AI.h"


AI::AI()
{
	mark = 'o';
}


AI::~AI()
{
}

int AI::minimax(Board *b, char m){
	int tmp, mmx;

	if (b->checkPlayerWin(m))
		return (m == 'o') ? 1 : -1;

	if (b->countBlank() == 0)
		return 0;

	m = (m == 'o') ? 'x' : 'o';

	mmx = (m == 'x') ? 10 : -10;

	// dla x - min dla o - max
	tmp = 0;
	for (int i = 0; i < 9; i++){
		if (b->board[i] == ' '){
			b->board[i] = m;
			tmp = minimax(b, m);
			b->board[i] = ' ';
			if (((m == 'x') && (tmp < mmx)) || ((m == 'o') && (tmp > mmx)))
				mmx = tmp;
		}
	}
	return mmx;

}

int AI::pickPosition(Board *b){

	int position, bestEver, tmp;

	tmp = 4;
	bestEver = -10;

	for (int i = 0; i < 9; i++){
		if (b->board[i] == ' '){
			b->board[i] = mark;
			tmp = minimax(b, mark);
			b->board[i] = ' ';
			if (tmp > bestEver){
				bestEver = tmp;
				position = i;
			}
		}
	}

	return position;
}