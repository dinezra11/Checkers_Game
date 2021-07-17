/* Author: Din Ezra, dinezra11@gmail.com

	This class is in charge of the user interface (UI).
	It includes the code for creating and rendering the menus of the game, and the information during the game.
*/

#ifndef _UI_H
#define _UI_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

class UI
{
private:
	SDL_Texture* titleTexture;
	SDL_Rect titlePosition;
	SDL_Rect turnPosition;
	SDL_Texture** turnTexture;
	const int PLAYERS = 2;

	// Private Methods:
	void initialize(SDL_Renderer*);

public:
	UI(SDL_Renderer*);
	~UI();

	void draw(SDL_Renderer*, int);
};

#endif