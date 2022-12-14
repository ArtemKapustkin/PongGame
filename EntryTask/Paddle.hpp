#pragma once
#include "SDL.h"
#include "Coordinates.hpp"
#include "Constants.hpp"
class Paddle
{
public:
	Paddle(Coordinates position, Coordinates velocity);

	void setStartCoordinates(Coordinates position, Coordinates velocity);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

	Coordinates position;
	Coordinates velocity;
	SDL_Rect rect{};
};

class BotPaddle : public Paddle
{
public:
	BotPaddle(Coordinates position, Coordinates velocity);
	void Tracking(Coordinates ball);
};