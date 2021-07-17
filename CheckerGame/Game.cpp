#include "Game.h"

void Game::initializeGameWindow()
{
	// Initialize the SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw SDL_GetError();

	// Create the game's window
	window = SDL_CreateWindow("Checker Game! by Din Ezra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
		throw SDL_GetError();

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		throw SDL_GetError();

	SDL_Surface* temp = IMG_Load("Images/winner.png");
	if (temp == nullptr)
		throw IMG_GetError();
	winTexture = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
}

void Game::input()
{
	SDL_Event evnt;

	if (SDL_PollEvent(&evnt)) {
		if (evnt.type == SDL_QUIT)
			state = State::Quit;
		else if (evnt.type == SDL_MOUSEBUTTONDOWN) {
			// Mouse button clicked event //
			int x, y;
			SDL_GetMouseState(&x, &y);
			convertMousePosToCell(x, y, &cellClicked.x, &cellClicked.y);

			if (state == State::Play) {
				focusedPiece = board->getPiecePointer(cellClicked.x, cellClicked.y);
				if (focusedPiece != nullptr)
					if (focusedPiece->getType() == turn) {
						// First step of the turn - Highlight the specific piece!
						focusedPiece->setFocused(true);
						state = State::OnMove;
					}
			}
			else if (state == State::OnMove) {
				if (!board->movePiece(focusedPiece, &cellClicked)) {
					// Cancel move
					focusedPiece->setFocused(false);
					board->cancelMove();
					state = State::Play;
				}
			}
		}
		else if (evnt.type == SDL_KEYDOWN) {
			// Keyboard's button pressed event //
			switch (evnt.key.keysym.sym) {
			case SDLK_ESCAPE:
				// Cancel move (ESC button has been pressed)
				if (state == State::OnMove) {
					focusedPiece->setFocused(false);
					board->cancelMove();
					state = State::Play;
				}
				break;
			case SDLK_SPACE:
				// Apply move (space button has been pressed)
				if (state == State::OnMove && (board->getStepsListSize() != 0)) {
					board->applyTurn(focusedPiece);
					switchTurn();
					focusedPiece->setFocused(false);
					state = State::Play;

					// Check for win after the move
					if (board->checkForWin())
						state = State::Win;
				}
				break;
			}
		}
	}
}

void Game::convertMousePosToCell(int mouseX, int mouseY, int* cellX, int* cellY)
{
	*cellX = (mouseX - BOARD_XOFFSET) / (BOARD_SIZE / 8);
	*cellY = (mouseY - BOARD_YOFFSET) / (BOARD_SIZE / 8);
}

void Game::switchTurn()
{
	if (turn == Color::Blue)
		turn = Color::Red;
	else
		turn = Color::Blue;
}

void Game::renderGame() const
{
	// Render the Game!
	SDL_SetRenderDrawColor(renderer, 0xB6, 0x8D, 0x40, 0xFF);
	SDL_RenderClear(renderer);
	board->draw(renderer);
	userInterface->draw(renderer, (int)turn);
	SDL_RenderPresent(renderer);
}

Game::Game()
{
	// Constructor - Initialize the game's components

	try {
		initializeGameWindow();

		board = new Board(renderer, BOARD_XOFFSET, BOARD_YOFFSET, BOARD_SIZE);
		turn = Color::Red;
		state = State::Play;

		cellClicked.x = 0;
		cellClicked.y = 0;

		userInterface = new UI(renderer);
	}
	catch (const char* e) {
		std::cout << "Error: " << e << std::endl;
		exit(-1);
	}
}

Game::~Game()
{
	// Destructor - Release dynamic memory allocated and clear the SDL library's components

	delete board;
	delete userInterface;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(winTexture);
	SDL_Quit();
}

void Game::run()
{
	while (state != State::Quit) {
		renderGame();

		if (state == State::Win) {
			SDL_Rect winnerRect = { WINDOW_WIDTH / 2 - 200,  WINDOW_HEIGHT / 2 - 80, 400, 140 };
			SDL_RenderCopy(renderer, winTexture, NULL, &winnerRect);
			SDL_RenderPresent(renderer);
			SDL_Delay(1500); // Wait for 1.5 seconds before closing..
			state = State::Quit;
		}

		input();
	}

	// Quitting the Game - Releasing dynamic memory allocated
	delete board;
	board = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyTexture(winTexture);
	winTexture = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
