/* Author: Din Ezra, dinezra11@gmail.com

	This class is the implementation of the Checker game. It includes all of the logic and rendering of the game.
	Initialize the game's components by creating an object of this class and calling its constructor.
	Use the method run() to run the game.
*/

#ifndef _GAME_H
#define _GAME_H

#include <iostream>
#include "Board.h"
#include "UI.h"

// Define Constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 900
#define BOARD_XOFFSET 0
#define BOARD_YOFFSET 100
#define BOARD_SIZE 800

enum class State {
	Play,
	OnMove,
	Win,
	Quit
};

class Game
{
private:
	// Game Components:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Board* board;
	State state;
	UI* userInterface;
	SDL_Texture* winTexture;

	// Turn and Input Components:
	Color turn;									// Indicated the current player's turn (for example: Blue or Red?)
	Position cellClicked;						// The cell that has been clicked by the player
	Piece* focusedPiece;						// A pointer to the focused piece

	// Private Methods:
	void initializeGameWindow();
	void input();
	void convertMousePosToCell(int, int, int*, int*);
	void switchTurn();
	void renderGame() const;

public:
	Game();
	~Game();

	void run();
};

#endif _GAME_H