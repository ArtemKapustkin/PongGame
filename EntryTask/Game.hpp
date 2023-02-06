#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include "Constants.hpp"
#include "Figures.hpp"
#include "Score.hpp"
#include <chrono>

class Game
{
public:
	Game(const char* title, int x, int y, int w, int h);

	~Game();
	
	void initFigures(GameMode gamemode);

	void printText(std::string name, int x, int y, int w, int h, TTF_Font* font);

	void handleEvents(SDL_Event* event);

	void renderMenu();

	void gameplay(GameMode gamemode, SDL_Event* event);

	bool IsRunning() const
	{
		return this->isRunning;
	}

	void setIsRunning(bool b)
	{
		this->isRunning = b;
	}

private:
	bool isRunning;
	Ball* ball;
	Paddle* paddleOne;
	Paddle* paddleTwo;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* scoreFont;
	TTF_Font* menuFont;
	SDL_Color White = { 255, 255, 255 };
	SDL_Surface* tmpSurface;
	SDL_Texture* tmpTexture;
	SDL_Rect rect;
};
