/* Author: Din Ezra, dinezra11@gmail.com

	This class represents the game's board. It includes all the pieces and basic logic.
*/

#ifndef _BOARD_H
#define _BOARD_H

#define DEFAULTSIZE 50
#define PIECESAMOUNT 12*2						// Each player has PIECESAMOUNT/2 pieces

#include "Piece.h"
#include <iostream>

class Board
{
private:
	// Board's Components
	SDL_Texture* background;
	Piece** pieces;
	int x,
		y,
		size;

	// Turn's Logic Components
	Piece** killList;							// Array of pointers that point to the pieces that need to be "killed"
	int killListSize;							// Size of the kill list
	Position** stepsList;						// Array of all the steps of the player's piece (to track path before applying a turn)
	int stepsListSize;							// Size of the steps list

	// Private Methods:
	void addStep(Position*);
	void addKill(Piece*);
	void applyKills();

	// Math's Methods:
	int absoluteValue(int);

public:
	Board(SDL_Renderer* renderer, int x = 0, int y = 0, int size = DEFAULTSIZE);
	~Board();

	Piece* getPiecePointer(int, int) const;
	bool isCellEmpty(int, int) const;
	int getStepsListSize() const { return stepsListSize; }

	bool movePiece(Piece*, Position*);
	void cancelMove();
	void applyTurn(Piece*);
	bool checkForWin() const;
	void draw(SDL_Renderer* renderer) const;
};

#endif _BOARD_H