#include "Board.h"

Board::Board(SDL_Renderer* renderer, int x, int y, int size)
{
	// Constructor

	this->background = nullptr;
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		throw IMG_GetError();
	else {
		SDL_Surface* surface = IMG_Load("Images/board.png");

		if (surface == nullptr)
			throw IMG_GetError();
		else {
			// Image loaded successfully!
			background = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}
	}
	this->x = x;
	this->y = y;
	this->size = size;

	// Initialize the Pieces
	try {
		pieces = new Piece * [PIECESAMOUNT];
		for (int i = 0; i < 4; i++) {
			pieces[i] = new Piece(renderer, Color::Blue, 1 + i * 2, 0, size / 8, size / 8);
			pieces[i + PIECESAMOUNT / 2] = new Piece(renderer, Color::Red, i * 2, 7, size / 8, size / 8);
		}
		for (int i = 4; i < 8; i++) {
			pieces[i] = new Piece(renderer, Color::Blue, (i - 4) * 2, 1, size / 8, size / 8);
			pieces[i + PIECESAMOUNT / 2] = new Piece(renderer, Color::Red, 1 + (i - 4) * 2, 6, size / 8, size / 8);
		}
		for (int i = 8; i < PIECESAMOUNT/2; i++) {
			pieces[i] = new Piece(renderer, Color::Blue, 1 + (i - 8) * 2, 2, size / 8, size / 8);
			pieces[i + PIECESAMOUNT / 2] = new Piece(renderer, Color::Red, (i - 8) * 2, 5, size / 8, size / 8);
		}
	}
	catch (...) {
		SDL_DestroyTexture(background);
		delete[] pieces;

		throw;
	}

	killList = nullptr;
	killListSize = 0;
	stepsList = nullptr;
	stepsListSize = 0;
}

Board::~Board()
{
	// Destructor

	for (int i = 0; i < PIECESAMOUNT; i++) {
		delete pieces[i];
	}
	delete[] pieces;

	for (int i = 0; i < killListSize; i++)
		delete killList[i];
	for (int i = 0; i < stepsListSize; i++)
		delete stepsList[i];
	delete[] killList;
	delete[] stepsList;

	SDL_DestroyTexture(background);
}

Piece* Board::getPiecePointer(int xCell, int yCell) const
{
	// Find the piece according to the cell's position and return its value.
	// Return nullptr if no piece has been found.

	for (int i = 0; i < PIECESAMOUNT; i++) {
		if (pieces[i]->isAlive())
			if ((pieces[i]->getXCell() == xCell) && (pieces[i]->getYCell() == yCell))
				return pieces[i];
	}

	return nullptr;
}

bool Board::isCellEmpty(int xCell, int yCell) const
{
	// Check if this specific cell is empty

	if (getPiecePointer(xCell, yCell))
		return false;

	return true;
}

bool Board::movePiece(Piece* currentPiece, Position* newPos)
{
	Piece* oppenentPtr = nullptr;					// Pointer to opponent's piece to kill
	int currentX, currentY;

	// Get current's position
	if (stepsListSize == 0) {
		currentX = currentPiece->getXCell();
		currentY = currentPiece->getYCell();
	}
	else {
		currentX = stepsList[stepsListSize - 1]->x;
		currentY = stepsList[stepsListSize - 1]->y;
	}

	if (newPos->x < 0 || newPos->x > 7 || newPos->y < 0 || newPos->y > 7)
		return false;

	if (!isCellEmpty(newPos->x, newPos->y))
		return false;

	// Attempts to move a piece. If failed, returns FALSE.
	if (currentPiece->isCrowned()) {
		////////// MOVE LOGIC FOR CROWNED PIECE //////////
		Position directionVector = { newPos->x - currentX, newPos->y - currentY };
		if (directionVector.x == 0 || directionVector.y == 0)
			return false;

		directionVector.x /= absoluteValue(directionVector.x); // Normalize direction's vector
		directionVector.y /= absoluteValue(directionVector.y); // Normalize direction's vector
		std::cout << "Direction Vector: " << directionVector.x << " " << directionVector.y << std::endl;
		if (absoluteValue(directionVector.x) != absoluteValue(directionVector.y))
			return false;

		// Check for an individual kill on the path
		int checkX = currentX + directionVector.x,
			checkY = currentY + directionVector.y;
		Piece* tempPtr = nullptr;

		while (checkX != newPos->x && checkY != newPos->y) {
			tempPtr = getPiecePointer(checkX, checkY);

			if (tempPtr != nullptr) {
				if (currentPiece->getType() == tempPtr->getType())
					return false;

				if (oppenentPtr == nullptr)
					oppenentPtr = tempPtr;
				else
					return false;
			}

			checkX += directionVector.x;
			checkY += directionVector.y;
		}

		if (stepsListSize != 0 && oppenentPtr == nullptr)
			return false;

		addStep(newPos);
		if (oppenentPtr)
			addKill(oppenentPtr);
	}
	else {
		////////// MOVE LOGIC FOR REGULAR PIECE //////////
		
		// Check move for regular step (single-cell step) - ***Available only for the first step of the turn!***
		if (stepsListSize == 0) {
			if ((newPos->x == currentX - 1 && newPos->y == currentY - 1)
				|| (newPos->x == currentX + 1 && newPos->y == currentY - 1)) {
				if (currentPiece->getType() == Color::Blue && !currentPiece->isCrowned())
					return false;

				addStep(newPos);
				return true;
			}
			else if ((newPos->x == currentX - 1 && newPos->y == currentY + 1)
				|| (newPos->x == currentX + 1 && newPos->y == currentY + 1)) {
				if (currentPiece->getType() == Color::Red && !currentPiece->isCrowned())
					return false;

				addStep(newPos);
				return true;
			}
		}

		// Check kill-step (multiple-cell steps, with killing an opponent's piece)
		if (newPos->x == currentX - 2 && newPos->y == currentY - 2)
			oppenentPtr = getPiecePointer(currentX - 1, currentY - 1);
		else if (newPos->x == currentX + 2 && newPos->y == currentY - 2)
			oppenentPtr = getPiecePointer(currentX + 1, currentY - 1);
		else if (newPos->x == currentX - 2 && newPos->y == currentY + 2)
			oppenentPtr = getPiecePointer(currentX - 1, currentY + 1);
		else if (newPos->x == currentX + 2 && newPos->y == currentY + 2)
			oppenentPtr = getPiecePointer(currentX + 1, currentY + 1);
		else
			return false;

		if (oppenentPtr == nullptr)
			return false;
		if (currentPiece->getType() == oppenentPtr->getType())
			return false;
		if (stepsListSize > 0 && killListSize == 0)
			return false;

		addStep(newPos);
		addKill(oppenentPtr);
	}

	return true;
}

void Board::cancelMove()
{
	// Cancel move

	for (int i = 0; i < stepsListSize; i++)
		delete stepsList[i];
	stepsListSize = 0;

	delete[] killList;
	killList = nullptr;
	killListSize = 0;
}

void Board::applyTurn(Piece* currentPiece)
{
	// Apply the turn! Commit the kills and update the current piece's position

	if (stepsListSize > 0) {
		currentPiece->setPosition(*stepsList[stepsListSize - 1]);
		stepsListSize = 0;
		delete[] stepsList;
		stepsList = nullptr;

		applyKills();

		// Check if the crown achieved
		if (currentPiece->getType() == Color::Blue && currentPiece->getYCell() == 7)
			currentPiece->setCrowned(true);
		if (currentPiece->getType() == Color::Red && currentPiece->getYCell() == 0)
			currentPiece->setCrowned(true);
	}
}

bool Board::checkForWin() const
{
	// Count how many pieces are still playing, and return if one of the players won
	int firstPlayer = 0,
		secondPlayer = 0;

	for (int i = 0; i < PIECESAMOUNT; i++)
		if (pieces[i]->isAlive())
			if (pieces[i]->getType() == Color::Blue)
				firstPlayer++;
			else
				secondPlayer++;

	if (firstPlayer == 0 || secondPlayer == 0)
		return true;

	return false;
}

void Board::addStep(Position* newPos)
{
	// Add a pointer to the kill list
	Position** tmp = new Position * [stepsListSize + 1];

	for (int i = 0; i < stepsListSize; i++) {
		tmp[i] = new Position(*stepsList[i]);
		delete stepsList[i];
		stepsList[i] = nullptr;
	}
	tmp[stepsListSize] = new Position(*newPos);

	stepsList = tmp;
	stepsListSize++;

}

void Board::addKill(Piece* piecePtr)
{
	// Add a pointer to the kill list
	Piece** tmp = new Piece * [killListSize + 1];
	
	for (int i = 0; i < killListSize; i++)
		tmp[i] = killList[i];
	tmp[killListSize] = piecePtr;

	killList = tmp;
	killListSize++;
}

void Board::applyKills()
{
	// Apply all the kills from the kill list

	for (int i = 0; i < killListSize; i++)
		killList[i]->setAlive(false);

	delete[] killList;
	killList = nullptr;
	killListSize = 0;
}

int Board::absoluteValue(int num)
{
	// Returns the absolute value of "num"
	if (num < 0)
		num *= -1;

	return num;
}

void Board::draw(SDL_Renderer* renderer) const
{
	// Draw the Board
	SDL_Rect rect = { x, y, size, size };
	SDL_RenderCopy(renderer, background, NULL, &rect);

	// Draw the Pieces
	for (int i = 0; i < PIECESAMOUNT; i++) {
		if (pieces[i]->isAlive())
			pieces[i]->draw(renderer, x, y);
	}

	// Draw the Steps
	for (int i = 0; i < stepsListSize; i++) {
		rect = { stepsList[i]->x * (size / 8) + x, stepsList[i]->y * (size / 8) + y, size/8, size/8 };
		
		for (int j = 0; j < 4; j++) { // <- Border of square will be 4 pixels
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF);
			SDL_RenderDrawRect(renderer, &rect);

			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
		}
	}
}
