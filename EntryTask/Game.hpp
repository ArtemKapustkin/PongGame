#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include "Constants.hpp"
#include "Figures.hpp"
#include "Score.hpp"
#include <chrono>


class Text
{
public:
	Text(std::string name, float x, float y, int w, int h, SDL_Renderer* renderer, TTF_Font* font)
	{
		const char* label = name.c_str();
		surface = TTF_RenderText_Solid(font, label, White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		// Rectangle coordinates and width/height
		rect.x = x;
		rect.y = y;
		rect.w = w;//T_WIDTH;
		rect.h = h;//T_HEIGHT;
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_RenderPresent(renderer);
	}

	~Text()
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}

	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Surface* surface{};
	SDL_Texture* texture{};
	SDL_Rect rect{};
	SDL_Color White = { 255, 255, 255 };
};


class Game
{
public:
	Game(const char* title, int x, int y, int w, int h, bool fullscreen);
	~Game();

	void init(const char* title, int x, int y, int w, int h, bool fullscreen);
	void handleEvents();
	void printtext(std::string name, int x, int y, int w, int h, SDL_Renderer* renderer, TTF_Font* font);
	void updateMenu();
	void updateGame();
	void renderMenu();
	void renderGame();
	void clear();

	bool running()
	{
		return isRunning;
	}

private:
	int count;
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	//TTF_Font* scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);
	//TTF_Font* menuFont = TTF_OpenFont("OpenSans-Regular.ttf", 15);
	SDL_Color White = { 255, 255, 255 };
	SDL_Surface* tmpSurface{};
	SDL_Texture* tmpTexture{};
	SDL_Rect rect{};
};
