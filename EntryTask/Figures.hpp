#pragma once
#include "SDL.h"
#include "Constants.hpp"
#include "Coordinates.hpp"

class Figure
{
public:
	Figure(Coordinates position, Coordinates velocity);
	//~Figure();

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

	Coordinates position;
	Coordinates velocity;
	SDL_Rect rect{};
};

class Paddle : public Figure
{
public:
	Paddle(Coordinates position, Coordinates velocity);

	void Draw(SDL_Renderer* renderer);

	void Update(float dt);

	void setStartCoordinates(Coordinates position, Coordinates velocity);
};

class Ball : public Figure
{
public:
	Ball(Coordinates position, Coordinates velocity);

	void CollideWithPaddle(Contact const& contact);

	void CollideWithWall(Contact const& contact);
};

class BotPaddle : public Paddle
{
public:
	BotPaddle(Coordinates position, Coordinates velocity);
	void Tracking(Coordinates ball);
};