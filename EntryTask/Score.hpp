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
	void SetScoreValue(int score);
	int GetScoreValue();
	void Draw();

private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect* rect;
	int scoreValue;
};