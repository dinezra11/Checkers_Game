#include "UI.h"

void UI::initialize(SDL_Renderer* renderer)
{
	// Initialize the "turn" text
	TTF_Font* font;
	SDL_Surface* tempSurface;
	SDL_Color color;

	if (TTF_Init() == -1)
		throw TTF_GetError();
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		throw IMG_GetError();

	font = TTF_OpenFont("Fonts/OpenSans-SemiBoldItalic.ttf", 24);
	color = { 0xFF,0xFF,0xFF };
	tempSurface = TTF_RenderText_Solid(font, "Turn: ", color);
	titleTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	titlePosition = { 5,20,80,50 };

	// Initialize the "turn" picture
	turnTexture = new SDL_Texture * [PLAYERS];
	color = { 0,0,0xFF };
	tempSurface = TTF_RenderText_Solid(font, "Blue", color);
	turnTexture[0] = SDL_CreateTextureFromSurface(renderer, tempSurface);
	turnPosition = { 90,20,50,50 };
	SDL_FreeSurface(tempSurface);

	color = { 0xFF,0,0 };
	tempSurface = TTF_RenderText_Solid(font, "Red", color);
	turnTexture[1] = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	TTF_CloseFont(font);
	TTF_Quit();
}

UI::UI(SDL_Renderer* renderer)
{
	initialize(renderer);
}

UI::~UI()
{
	SDL_DestroyTexture(titleTexture);
	
	for (int i = 0; i < PLAYERS; i++)
		SDL_DestroyTexture(turnTexture[i]);
	delete[] turnTexture;
}

void UI::draw(SDL_Renderer* renderer, int turn)
{
	// Render the UI
	SDL_RenderCopy(renderer, titleTexture, NULL, &titlePosition);

	if (turn >= 0 && turn < PLAYERS)
		SDL_RenderCopy(renderer, turnTexture[turn], NULL, &turnPosition);
}
