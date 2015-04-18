#include "Board.h"


Board::Board()
{
	for (int i = 0; i < sizeof(board); i++){
		board[i] = ' ';
	}
	emptySpace = 9;
}


Board::~Board()
{
}

void Board::draw(){
	std::cout << " ============= Console X-O game ============= " << std::endl << std::endl;

	for (int i = 0; i < sizeof(board); i++){
		if (i % 3 == 0 && i != 0){
			std::cout << std::endl << "------" << std::endl;
		}
		if (i % 3 != 2)
			std::cout << board[i] << "|";
		else
			std::cout << board[i];
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void Board::drawGL(int w, int h, void(*dispTxt)(std::string txt, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB)){
	int x = 0;
	int y = 0;
	for (int i = 0; i < sizeof(board); i++){
		x = (i % 3) * (w / 3) + 50;
		y = (i / 3) * (h / 3) + 50;
		std::stringstream ss;
		std::string tmp;
		ss << board[i];
		ss >> tmp;
		dispTxt(tmp, x, y, 100, 255, 0, 0, 0, 0, 0);
	}
}

void Board::put(int position, char mark){
	board[position] = mark;
	emptySpace--;
}

bool Board::checkAvailability(int position){
	if (board[position] != ' ')
		return false;
	else
		return true;
}

bool Board::checkEndGameConditions(){
	// edges
	if (board[1] != ' ' && board[1] == board[0] && board[1] == board[2])
		return true;
	if (board[3] != ' ' && board[3] == board[0] && board[3] == board[6])
		return true;
	if (board[5] != ' ' && board[5] == board[2] && board[5] == board[8])
		return true;
	if (board[7] != ' ' && board[7] == board[6] && board[7] == board[8])
		return true;

	// + and x config
	if (board[4] != ' ' && board[4] == board[1] && board[4] == board[7])
		return true;
	if (board[4] != ' ' && board[4] == board[3] && board[4] == board[5])
		return true;
	if (board[4] != ' ' && board[4] == board[0] && board[4] == board[8])
		return true;
	if (board[4] != ' ' && board[4] == board[2] && board[4] == board[6])
		return true;

	return false;
}

bool Board::checkPlayerWin(char mark){
	// edges
	if (board[1] == mark && board[1] == board[0] && board[1] == board[2])
		return true;
	if (board[3] == mark && board[3] == board[0] && board[3] == board[6])
		return true;
	if (board[5] == mark && board[5] == board[2] && board[5] == board[8])
		return true;
	if (board[7] == mark && board[7] == board[6] && board[7] == board[8])
		return true;

	// + and x config
	if (board[4] == mark && board[4] == board[1] && board[4] == board[7])
		return true;
	if (board[4] == mark && board[4] == board[3] && board[4] == board[5])
		return true;
	if (board[4] == mark && board[4] == board[0] && board[4] == board[8])
		return true;
	if (board[4] == mark && board[4] == board[2] && board[4] == board[6])
		return true;

	return false;
}

int Board::countBlank(){
	int tmp = 0;
	for (int i = 0; i < 9; i++){
		if (board[i] == ' ')
			tmp++;
	}
	return tmp;
}