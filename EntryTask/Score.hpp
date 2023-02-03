#pragma once
#include"SDL.h"
#include"SDL_ttf.h"
#include"Coordinates.hpp"
#include<string>
class Score
{
public:
	Score(Coordinates position, SDL_Renderer* renderer, TTF_Font* font);
	~Score();
	void SetScore(int score);
	void Draw();

private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
};