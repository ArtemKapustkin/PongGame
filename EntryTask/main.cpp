#include "SDL_ttf.h"
#include "SDL.h"
#include "Game.hpp"

Game *game = nullptr;

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	
	Uint32 frameStart;
	int frameTime;
	
	game = new Game("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT);
	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->updateMenu();
		game->renderMenu();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	game->~Game();
	//Game game("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, false);
	//game.start();
	return 0;
}