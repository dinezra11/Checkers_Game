#include "Piece.h"

Piece::Piece(SDL_Renderer* renderer, Color type, int xCell, int yCell, int width, int height)
{
	// Constructor

	this->texture = nullptr;
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		throw IMG_GetError();
	else {
		SDL_Surface* surface;

		// Load base Texture
		if (type == Color::Blue)
			surface = IMG_Load("Images/blue.png");
		else
			surface = IMG_Load("Images/red.png");

		if (surface == nullptr)
			throw IMG_GetError();
		else {
			// Image loaded successfully!
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF)); // Set white color to be transparent
			texture = SDL_CreateTextureFromSurface(renderer, surface);
		}

		// Load crown Texture
		surface = IMG_Load("Images/crown.png");

		if (surface == nullptr)
			throw IMG_GetError();
		else {
			// Image loaded successfully!
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF)); // Set white color to be transparent, same as above
			crown = SDL_CreateTextureFromSurface(renderer, surface);
		}

		SDL_FreeSurface(surface);
	}

	this->type = type;
	this->setPosition(xCell, yCell);
	this->width = width;
	this->height = height;

	this->focused = false;
	this->crowned = false;
	this->alive = true;
}

Piece::~Piece()
{
	// Destructor

	SDL_DestroyTexture(texture);
}

void Piece::setPosition(int xCell, int yCell)
{
	// Set the piece's position (must be inside board's bounds - 8x8 cells)

	if (xCell < 0 || xCell>7)
		this->position.x = 0;
	else
		this->position.x = xCell;

	if (yCell < 0 || yCell>7)
		this->position.y = 0;
	else
		this->position.y = yCell;
}

void Piece::setPosition(const Position& newPos)
{
	// Set the piece's position (must be inside board's bounds - 8x8 cells)

	if (newPos.x < 0 || newPos.x > 7)
		this->position.x = 0;
	else
		this->position.x = newPos.x;

	if (newPos.y < 0 || newPos.y > 7)
		this->position.y = 0;
	else
		this->position.y = newPos.y;
}

void Piece::setFocused(bool flag)
{
	focused = flag;
}

void Piece::setCrowned(bool flag)
{
	crowned = flag;
}

void Piece::setAlive(bool flag)
{
	alive = flag;
}

void Piece::draw(SDL_Renderer* renderer, int xOffset, int yOffset) const
{
	// Calculate real positions (in pixels)
	SDL_Rect rect;
	int xCoord = position.x * width + xOffset;
	int yCoord = position.y * height + yOffset;
	rect = { xCoord,yCoord,width,height };

	// Render the piece
	if (focused) {
		SDL_SetTextureAlphaMod(texture, 0x90);
		SDL_SetTextureAlphaMod(crown, 0x90);
	}
	else {
		SDL_SetTextureAlphaMod(texture, 0xFF);
		SDL_SetTextureAlphaMod(crown, 0xFF);
	}
	SDL_RenderCopy(renderer, texture, NULL, &rect);

	// Render crown, if needed
	if (crowned)
		SDL_RenderCopy(renderer, crown, NULL, &rect);
}
