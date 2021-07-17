/* Author: Din Ezra, dinezra11@gmail.com

	This class represents a single piece of the game.
*/

#ifndef _PIECE_H
#define _PIECE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

enum class Color {
	Blue,
	Red
};

struct Position {
	int x;
	int y;
};

class Piece
{
private:
	SDL_Texture* texture;
	SDL_Texture* crown;
	Color type;
	Position position;					// Position in relative to the board's cells
	int	width,
		height;
	bool focused,						// focused = TRUE if this piece is currently activated. (If has been clicked by the player)
		crowned,						// crowned = TRUE if this piece got crown! (super piece with additional abilities)
		alive;							// alive = TRUE if this piece is still in the game.

public:
	Piece(SDL_Renderer* renderer, Color type, int xCell = 0, int yCell = 0, int width = 10, int height = 10);
	~Piece();

	int getXCell() const { return position.x;}
	int getYCell() const { return position.y; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	Color getType() const { return type; }
	bool isAlive() const { return alive; }
	bool isCrowned() const { return crowned; }

	void setPosition(int, int);
	void setPosition(const Position&);
	void setFocused(bool);
	void setCrowned(bool);
	void setAlive(bool);

	void draw(SDL_Renderer*, int xOffset = 0, int yOffset = 0) const;
};

#endif