#include "SDL_ttf.h"
#include "SDL.h"
#include "Game.hpp"

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	
	Uint32 frameStart;
	int frameTime;
	SDL_Event event;
	Game* game = new Game("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT);
	while (game->IsRunning())
	{
		frameStart = SDL_GetTicks();

		game->HandleEvents(&event);

		game->RenderMenu();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}
	game->~Game();
	return 0;
}