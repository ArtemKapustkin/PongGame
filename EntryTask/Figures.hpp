#pragma once
#include "SDL.h"
#include<iostream>
#include "Constants.hpp"
#include "Coordinates.hpp"

class Figure
{
public:
	Figure(Coordinates position, Coordinates velocity);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

	Coordinates position;
	Coordinates velocity;
	SDL_Rect rect;
};

class Paddle : public Figure
{
public:
	Paddle(Coordinates position, Coordinates velocity);

	void Draw(SDL_Renderer* renderer);

	void Update(float dt);

	void SetStartCoordinates(Coordinates position, Coordinates velocity);

	virtual void Tracking(Coordinates ball) {
		std::cout << "Tracking" << std::endl;
	};
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