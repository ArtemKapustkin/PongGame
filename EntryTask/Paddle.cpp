#include"Paddle.hpp"
#include<iostream>
Paddle::Paddle(Coordinates position, Coordinates velocity) : position(position), velocity(velocity)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);
	rect.w = P_WIDTH;
	rect.h = P_HEIGHT;
}

void Paddle::setStartCoordinates(Coordinates position, Coordinates velocity)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);
}

void Paddle::Update(float dt)
{
	position += velocity * dt;

	if (position.y < 0)
	{
		// Restrict to top of the screen
		position.y = 0;
	}
	else if (position.y > (W_HEIGHT - P_HEIGHT))
	{
		// Restrict to bottom of the screen
		position.y = W_HEIGHT - P_HEIGHT;
	}
}

void Paddle::Draw(SDL_Renderer* renderer)
{
	rect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(renderer, &rect);
}

BotPaddle::BotPaddle(Coordinates position, Coordinates velocity) : Paddle(position, velocity)
{
}

void BotPaddle::Tracking(Coordinates ball)
{
	position.x = W_WIDTH - 50;
	if (ball.y > position.y)
	{
		//std::cout << "Going down " << std::endl;
		velocity.y = P_AI_SPEED;
	}

	else if (ball.y <= position.y)
	{
		//std::cout << "Going up " << std::endl;
		velocity.y = -P_AI_SPEED;
	}


}
