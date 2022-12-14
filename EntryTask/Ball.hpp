#pragma once
#include "SDL.h"
#include "Constants.hpp"
#include "Coordinates.hpp"

class Ball
{
public:
	Ball(Coordinates position, Coordinates velocity);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

	void CollideWithPaddle(Contact const& contact);

	void CollideWithWall(Contact const& contact);

	Coordinates position;
	Coordinates velocity;
	SDL_Rect rect{};
}; 
