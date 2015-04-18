#include <iostream>
#include <stdio.h>  
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stack>
#include "Board.h"
#include "AI.h"

// defs for quick changes
#define Window_Height 480
#define Window_Wigth 480
#define Window_Cap "Tic tac toe"

#define Frames_per_sec 30
#define Frame_rate 1000/Frames_per_sec

using namespace std;

struct StateStruct
{
	void(*StatePointer)();
};

// global vars
stack<StateStruct> game_StateStack;		// here is a stack for game states
SDL_Window* game_Window = NULL;			// game window
SDL_Surface* game_Sufrace = NULL;		// back buffer
SDL_Renderer* renderer = NULL;
SDL_Event game_Event;					// from here i can get input
int last_tick;							// timer for frame rate
Board* board = NULL;					// board of our game
AI* computerPlayer = NULL;				// our oponent for the game, you cant win is he's name ;-)
int whoWon;								// flag to tell that AI won, player won or it was draw

// Functions to handle game state
void Menu();
void Game();
void Exit();

// Function for initialization and closeing
void Init();
void CleanUp();

// Helpers for game states
void clearDisp();
void dispTxt(string txt, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB);
void handleMenuInput();
void handleGameInput();
void handleExitInput();
void renderMenu();
void renderGame();
void renderExit();
void newGame();
int convertXYtoPos(int x, int y); // convert position of mouse click to place in game array

int main(int argc, char** argv){

	Init();

	while (!game_StateStack.empty()){
		game_StateStack.top().StatePointer();
	}

	CleanUp();
	exit(EXIT_SUCCESS);
}

void Init(){
	// Init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	// set up window dimension
	game_Window = SDL_CreateWindow(Window_Cap, 1600, 100, Window_Wigth, Window_Height, SDL_WINDOW_SHOWN);

	// set black background
	game_Sufrace = SDL_GetWindowSurface(game_Window);
	renderer = SDL_CreateRenderer(game_Window, 0, SDL_RENDERER_ACCELERATED);
	clearDisp();

	// remember the time so can get prefer frame rate
	last_tick = SDL_GetTicks();

	// brand new game board
	board = new Board();

	// get me a new droid to play with 
	computerPlayer = new AI();
	computerPlayer->mark = 'o';

	// no one win at the moment
	whoWon = -1;

	// exit state always on bottom
	StateStruct state;
	state.StatePointer = Exit;
	game_StateStack.push(state);

	// as game start in menu so menu state must be loaded
	state.StatePointer = Menu;
	game_StateStack.push(state);

	// Init fonts lib
	TTF_Init();

}

void CleanUp(){

	// fonts lib off
	TTF_Quit();

	// free surface & window
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(game_Sufrace);
	SDL_DestroyWindow(game_Window);

	// quit sdl
	SDL_Quit();
}

void clearDisp(){
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderClear(renderer);

}

void dispTxt(string txt, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB){
	// prepare font
	TTF_Font* font = TTF_OpenFont("arial.ttf", 28);

	SDL_Color foreground = { fR, fG, fB };
	SDL_Color background = { bR, bG, bB };

	// temp surface for store our txt
	SDL_Surface* temp = TTF_RenderText_Blended(font, txt.c_str(), foreground);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);

	// prepare and render it
	SDL_Rect rect = { x, y, 100, 100 };
	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
	SDL_RenderCopy(renderer, texture, NULL, &rect);	

	// always clean up after you ;-)
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(temp);
	//SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

}

void Menu(){
	newGame();
	if ((SDL_GetTicks() - last_tick) >= Frame_rate){
		handleMenuInput();
		clearDisp();
		renderMenu();
		last_tick = SDL_GetTicks();
	}
}

void Game(){
	if ((SDL_GetTicks() - last_tick) >= Frame_rate){

		handleGameInput();

		if (board->checkPlayerWin('x'))
			whoWon = 1;
		else if (board->checkPlayerWin('o'))
			whoWon = 2;
		else if (board->emptySpace == 0)
			whoWon = 0;

		clearDisp();

		renderGame();

		last_tick = SDL_GetTicks();
	}
}

void Exit(){
	if ((SDL_GetTicks() - last_tick) >= Frame_rate){
		clearDisp();
		renderExit();
		handleExitInput();
		last_tick = SDL_GetTicks();
	}
}

void handleMenuInput(){
	// if red 'x' in right corner was hit
	if (SDL_PollEvent(&game_Event)){
		if (game_Event.type == SDL_QUIT){
			while (!game_StateStack.empty())
				game_StateStack.pop();
			return;
		}
	}
	if (game_Event.type == SDL_KEYDOWN){
		// escape or e to quit game ;-)
		if (game_Event.key.keysym.sym == SDLK_e){
			game_StateStack.pop();
			return;
		}
		// p to play
		if (game_Event.key.keysym.sym == SDLK_p){
			StateStruct state;
			state.StatePointer = Game;
			game_StateStack.push(state);
			return;
		}
	}
}

void handleExitInput(){
	if (SDL_PollEvent(&game_Event)){
		// red x
		if (game_Event.type == SDL_QUIT){
			while (!game_StateStack.empty())
				game_StateStack.pop();
			return;
		}
		// key down
		if (game_Event.type == SDL_KEYDOWN){
			// esc or y
			if (game_Event.key.keysym.sym == SDLK_ESCAPE || game_Event.key.keysym.sym == SDLK_y){
				game_StateStack.pop();
				return;
			}
			// n so back to menu
			if (game_Event.key.keysym.sym == SDLK_n){
				StateStruct state;
				state.StatePointer = Menu;
				game_StateStack.push(state);
				return;
			}

		}
	}	
}

void handleGameInput(){
	if (SDL_PollEvent(&game_Event)){
		// red x
		if (game_Event.type == SDL_QUIT){
			while (!game_StateStack.empty())
				game_StateStack.pop();
			return;
		}
		// esc so quit to menu
		if (game_Event.type == SDL_KEYDOWN){
			if (game_Event.key.keysym.sym == SDLK_ESCAPE){
				game_StateStack.pop();
				return;
			}
			// r so make new game
			if (game_Event.key.keysym.sym == SDLK_r){
				newGame();
				return;
			}
		}
		if (game_Event.type == SDL_MOUSEBUTTONDOWN && !board->checkEndGameConditions()){
			if (game_Event.button.button == SDL_BUTTON_LEFT){
				//cout << "x " << game_Event.button.x << " y " << game_Event.button.y << " " << convertXYtoPos(game_Event.button.x, game_Event.button.y) << endl;
				int pos = convertXYtoPos(game_Event.button.x, game_Event.button.y);
				// check if can place player mark here
				if (board->checkAvailability(pos)){
					// if so place it
					board->put(pos, 'x');
					// and let the computer pick where to place his mark & put it there cus computer are never wrong ;-)
					if (board->emptySpace != 0)
						board->put(computerPlayer->pickPosition(board), computerPlayer->mark);
				}					
			}
		}
	}
}

void renderMenu(){
	dispTxt("(P)lay", 100, 0, 12, 0, 0, 0, 255, 0, 0);
	dispTxt("(E)xit", 100, 50, 12, 0, 0, 0, 255, 0, 0);
	// display current game window
	SDL_RenderPresent(renderer);
}

void renderGame(){
	// prepare lines of the game grid
	SDL_Rect vertical_1 = { (Window_Wigth / 3) - 5, 40, 10, Window_Height };
	SDL_Rect vertical_2 = { ((Window_Wigth * 2) / 3) - 5, 40, 10, Window_Height };
	SDL_Rect horizontal_1 = { 0, (Window_Height / 3) - 5, Window_Wigth, 10 };
	SDL_Rect horizontal_2 = { 0, ((Window_Height * 2) / 3) - 5, Window_Wigth, 10 };
	SDL_Rect lines[4] = { vertical_1, vertical_2, horizontal_1, horizontal_2 }; // store them in an array for quicker drawing
	// set color of lines & draw lines ona a renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRects(renderer, lines, 4);
	// display info which sign is player
	dispTxt("You play as 'X' . ESC - menu, R - new", 0, 0, 1, 0, 0, 0, 255, 0, 0);
	// draw info if and who won a game
	if ( whoWon == 1)
		dispTxt("Congratz! You won.", 100, Window_Height/2, 1, 0, 0, 255, 255, 0, 0);
	else if ( whoWon == 2)
		dispTxt("Buuuu! Computer won.", 100, Window_Height / 2, 1, 0, 0, 255, 255, 0, 0);
	else if ( whoWon == 0)
		dispTxt("Nice play! It is a draw.", 100, Window_Height / 2, 1, 0, 0, 255, 255, 0, 0);
	// draw game board in consol & our window
	board->draw();
	board->drawGL(Window_Wigth, Window_Height, dispTxt);
	// display current game window
	SDL_RenderPresent(renderer);
}

void renderExit(){
	dispTxt("Do you wanna quit? (y) or (n)", 50, 111, 12, 0, 0, 0, 255, 0, 0);
	// display current game window
	SDL_RenderPresent(renderer);
}

void newGame(){
	delete board;
	board = new Board();
	// no one win at the moment
	whoWon = -1;
}

int convertXYtoPos(int x, int y){
	int pos = -1;
	// pos = ySTH*3 + xSTH
	pos = (y/(Window_Height/3)) * 3 + (x/(Window_Wigth/3));
	return pos;
}
